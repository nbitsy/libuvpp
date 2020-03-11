
#include "UVIODevice.h"
#include "UVDataHelper.h"
#include "UVReqWrite.h"
#include "UVLoop.h"

namespace XNode
{

static void __OnRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    UVData *data = (UVData *)stream->data;
    if (NULL == data)
    {
        UVDataHelper::BufFree(buf, NULL);
        return;
    }

    UVIODevice *uviodevice = (UVIODevice *)data->_self;
    if (NULL == uviodevice)
    {
        UVDataHelper::BufFree(buf, NULL);
        return;
    }

    if (nread <= 0)
    {
        uviodevice->Close();
        UVDataHelper::BufFree(buf, uviodevice->GetLoop());
        return;
    }

    uviodevice->OnRead(buf->base, nread);
    UVDataHelper::BufFree(buf, uviodevice->GetLoop());
}

static void __OnReadUDP(uv_udp_t *stream, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
    UVData *data = (UVData *)stream->data;
    if (NULL == data)
    {
        UVDataHelper::BufFree(buf, NULL);
        return;
    }

    UVIODevice *uviodevice = (UVIODevice *)data->_self;
    if (NULL == uviodevice)
    {
        UVDataHelper::BufFree(buf, NULL);
        return;
    }

    if (nread <= 0)
    {
        if (nread < 0)
            uviodevice->Close();

        UVDataHelper::BufFree(buf, uviodevice->GetLoop());
        return;
    }

    uviodevice->OnRead(buf->base, nread, addr, flags);
    UVDataHelper::BufFree(buf, uviodevice->GetLoop());
}

bool UVIODevice::Bind(uv_handle_t *handle, const std::string &ip, int port, unsigned int flags)
{
    if (NULL == handle)
        return false;

    struct sockaddr *addr = NULL;
    if (_flags == AF_INET6)
    {
        uv_ip6_addr(ip.c_str(), port, &_addr6);
        addr = (struct sockaddr *)&_addr6;
    }
    else
    {
        uv_ip4_addr(ip.c_str(), port, &_addr);
        addr = (struct sockaddr *)&_addr;
    }

    if (NULL == addr)
        return false;

    if (handle->type == UV_TCP)
    {
        if (!uv_tcp_bind((uv_tcp_t *)handle, addr, flags))
            return true;
    }
    else if (handle->type == UV_UDP)
    {
        if (!uv_udp_bind((uv_udp_t *)handle, addr, flags))
            return true;
    }

    return false;
}

int UVIODevice::GetAf() const
{
    return _addr.sin_family;
}

void UVIODevice::InitAddress()
{
    struct sockaddr_in *addr = NULL;
    int len = sizeof(_addr);
    int af = GetAf();
    if (af == AF_INET6)
    {
        addr = (struct sockaddr_in *)&_addr6;
        len = sizeof(_addr6);
    }
    else
        addr = (struct sockaddr_in *)&_addr;

    if (_handle->type == UV_TCP)
        uv_tcp_getsockname(GetHandle<uv_tcp_t>(), (struct sockaddr *)addr, &len);
    else if (_handle->type == UV_UDP)
        uv_udp_getsockname(GetHandle<uv_udp_t>(), (struct sockaddr *)addr, &len);

    len = sizeof(_addr);
    if (af == AF_INET6)
    {
        addr = (struct sockaddr_in *)&_addr6Peer;
        len = sizeof(_addr6);
    }
    else
        addr = (struct sockaddr_in *)&_addrPeer;

    if (_handle->type == UV_TCP)
        uv_tcp_getpeername(GetHandle<uv_tcp_t>(), (struct sockaddr *)addr, &len);
    else if (_handle->type == UV_UDP)
        uv_udp_getpeername(GetHandle<uv_udp_t>(), (struct sockaddr *)addr, &len);
}

void UVIODevice::GetAddress(int type, EndPointAddress &address, const struct sockaddr *raddr) const
{
    char ip[128] = {
        0,
    };

    const struct sockaddr_in *addr = NULL;
    if (raddr != NULL)
    {
        addr = (const struct sockaddr_in *)raddr;
    }
    else
    {
        if (type == 0)
        {
            addr = &_addr;
            if (addr->sin_family == AF_INET6)
                addr = (const struct sockaddr_in *)&_addr6;
        }
        else
        {
            addr = &_addrPeer;
            if (addr->sin_family == AF_INET6)
                addr = (const struct sockaddr_in *)&_addr6Peer;
        }
    }

    if (addr->sin_family == AF_INET)
        uv_ip4_name(addr, ip, sizeof(ip));
    else if (_addr.sin_family == AF_INET6)
        uv_ip6_name((const struct sockaddr_in6 *)addr, ip, sizeof(ip));

    address._ip = ip;
    address._port = ntohs(addr->sin_port);
}

const EndPointAddress UVIODevice::LocalAddress() const
{
    EndPointAddress address;
    GetAddress(0, address);
    return address;
}

const EndPointAddress UVIODevice::RemoteAddress(const struct sockaddr *remote) const
{
    EndPointAddress address;
    GetAddress(1, address, remote);
    return address;
}

bool UVIODevice::StartRead()
{
    if (NULL == _handle)
        return false;

    if (_handle->type == UV_TCP)
    {
        int ret = uv_read_start(GetHandle<uv_stream_t>(), UVDataHelper::BufAlloc, __OnRead);
        if (ret != 0)
            return false;
    }
    else if (_handle->type == UV_UDP)
    {
        int ret = uv_udp_recv_start(GetHandle<uv_udp_t>(), UVDataHelper::BufAlloc, __OnReadUDP);
        if (ret != 0)
            return false;
    }

    return true;
}

bool UVIODevice::StopRead()
{
    if (NULL == _handle)
        return false;

    if (_handle->type == UV_TCP)
        return uv_read_stop(GetHandle<uv_stream_t>()) == 0;
    else if (_handle->type == UV_UDP)
        return uv_udp_recv_stop(GetHandle<uv_udp_t>()) == 0;

    return false;
}

bool UVIODevice::Write(void *data, int nsize, UVIODevice *other, const struct sockaddr *addr)
{
    if (NULL == _handle)
        return false;

    auto loop = GetLoop();
    if (NULL == loop)
    {
        DEBUG("*********************loop is null!!!***********************");
        return false;
    }

    UVReqWrite *req = loop->Construct<UVReqWrite>();
    if (req != NULL)
    {
        req->Init(this, other, data, nsize);
        return req->Start();
    }

    return false;
}

bool UVIODevice::Write(void *bufs[], int nbuf, UVIODevice *other, const struct sockaddr *addr)
{
    if (NULL == _handle)
        return false;

    auto loop = GetLoop();
    if (NULL == loop)
    {
        DEBUG("*********************loop is null!!!***********************");
        return false;
    }

    UVReqWrite *req = loop->Construct<UVReqWrite>();
    if (req != NULL)
    {
        req->Init(this, other, bufs, nbuf);
        return req->Start();
    }

    return false;
}

bool UVIODevice::TryWrite(void *data, int nsize, const struct sockaddr *addr)
{
    if (NULL == _handle)
        return false;

    uv_buf_t buf = uv_buf_init((char *)data, nsize);
    if (NULL == addr)
        return uv_try_write(GetHandle<uv_stream_t>(), &buf, 1) == 0;
    else if (_handle->type == UV_UDP)
        return uv_udp_try_send(GetHandle<uv_udp_t>(), &buf, 1, addr) == 0;

    return false;
}

bool UVIODevice::TryWrite(void *bufs[], int nbuf, const struct sockaddr *addr)
{
    if (NULL == _handle)
        return false;

    if (nbuf <= 0 || NULL == bufs)
        return false;

    uv_buf_t *uvbufs = (uv_buf_t *)Allocator::malloc(nbuf * sizeof(uv_buf_t));
    if (NULL == uvbufs)
        return false;

    for (int i = 0; i < nbuf; ++i)
    {
        void *data = bufs[i];
        int size = *(int *)data;
        uvbufs[i] = uv_buf_init((char *)data + sizeof(size), size);
    }

    if (NULL == addr)
        return uv_try_write(GetHandle<uv_stream_t>(), uvbufs, nbuf) == 0;
    else if (_handle->type == UV_UDP)
        return uv_udp_try_send(GetHandle<uv_udp_t>(), uvbufs, nbuf, addr) == 0;

    return false;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
