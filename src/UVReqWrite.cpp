
#include "UVReqWrite.h"
#include "Allocator.h"
#include "UVHandle.h"
#include "UVLoop.h"
#include <iostream>

namespace XSpace
{

static void __OnWrite(uv_write_t *req, int status)
{
    DEBUG("status: %d\n", status);
    if (NULL == req)
        return;

    if (status < 0)
    {
        if (req->handle != NULL)
        {
            // XXX: _iohandle
            UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)req->handle);
            if (uvdata != NULL && uvdata->_self != NULL)
            {
                auto handle = uvdata->GetPtr<UVHandle>();
                if (handle != NULL)
                    handle->Close();
            }
        }
    }

    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;

    UVReqWrite *self = uvdata->GetPtr<UVReqWrite>();
    if (NULL == self)
        return;

    // OnReq必须在最后面被调用
    self->OnReq(status);

    // 如果Req里注册的是强引用，则释放引用，一般来说Req使用完后需要回收
    auto strong = uvdata->GetStrongPtr<UVReqWrite>();
    if (strong != NULL)
        strong->reset();
}

static void __OnWriteUDP(uv_udp_send_t *req, int status)
{
    if (NULL == req)
        return;

    if (status < 0)
    {
        if (req->handle != NULL)
        {
            UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)req->handle);
            if (uvdata != NULL && uvdata->_self != NULL)
            {
                auto device = uvdata->GetPtr<UVHandle>();
                if (device != NULL)
                    device->Close();
            }
        }
    }

    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;

    UVReqWrite *self = uvdata->GetPtr<UVReqWrite>();
    if (NULL == self)
        return;

    // OnReq必须在最后面被调用
    self->OnReq(status);
}

void UVReqWrite::Init(std::weak_ptr<UVHandle> &iohandle, void *data, int nsize, bool copy)
{
    d._data = data;
    d._nsize = nsize;

    if (copy && data != NULL && nsize > 0)
    {
        d._data = Allocator::malloc(nsize);
        if (d._data != NULL)
        {
            _bCopye = true;
            memcpy(d._data, data, nsize);
        }
    }

    InitReq(iohandle);
}

void UVReqWrite::InitReq(std::weak_ptr<UVHandle> &iohandle)
{
    auto device = iohandle.lock();
    if (NULL == device)
        return;

    uv_handle_t *handle = device->GetHandle<uv_handle_t>();
    if (NULL == handle)
        return;

    if (handle->type == UV_TCP || handle->type == UV_TTY)
    {
        _req = (uv_req_t *)Allocator::Construct<uv_write_t>();
    }
    else if (handle->type == UV_UDP)
    {
        _req = (uv_req_t *)Allocator::Construct<uv_udp_send_t>();
    }
    else
    {
        ERROR("^^^^^^^^^^^^^Unkonw handle!!!^^^^^^^^^^^^^\n");
    }

    if (_req != NULL)
        uv_req_set_data(_req, NULL);
}

void UVReqWrite::Init(std::weak_ptr<UVHandle> &iohandle, void *bufs[], int nbuf, bool copy)
{
    d2._bufs = bufs;
    d2._nbuf = nbuf;

    if (copy && bufs != NULL && nbuf > 0)
    {
        d2._bufs = (void **)Allocator::malloc(nbuf * sizeof(void *));
        if (d2._bufs != NULL)
        {
            _bCopye = true;
            for (int i = 0; i < nbuf; ++i)
            {
                int size = *((int *)bufs[i]);
                void *buf = (void *)Allocator::malloc(size + sizeof(size));

                if (buf)
                    memcpy(buf, bufs[i], size + sizeof(size));

                d2._bufs[i] = buf;
            }
        }
    }

    InitReq(iohandle);
}

UVReqWrite::UVReqWrite(std::weak_ptr<UVHandle> &iohandle, std::weak_ptr<UVHandle> &other, void *data, int nsize, bool copy)
    : UVReq(), _bCopye(copy), _iohandle(iohandle), _other(other), _bBuffers(false)
{
    DEBUG("Object @%p\n", this);
    Init(iohandle, data, nsize, copy);
}

UVReqWrite::UVReqWrite(std::weak_ptr<UVHandle> &iohandle, std::weak_ptr<UVHandle> &other, void *bufs[], int nbuf, bool copy)
    : UVReq(), _bCopye(false), _iohandle(iohandle), _other(other), _bBuffers(true)
{
    DEBUG("Object @%p\n", this);
    Init(iohandle, bufs, nbuf, copy);
}

UVReqWrite::UVReqWrite(std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *data, int nsize, bool copy)
    : UVReq(), _bCopye(false), _iohandle(iohandle), _bBuffers(false), _addr(NULL)
{
    DEBUG("Object @%p\n", this);
    Init(iohandle, data, nsize, copy);
    InitAddress(addr);
}

UVReqWrite::UVReqWrite(std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *bufs[], int nbuf, bool copy)
    : UVReq(), _bCopye(false), _iohandle(iohandle), _bBuffers(true), _addr(NULL)
{
    DEBUG("Object @%p\n", this);
    Init(iohandle, bufs, nbuf, copy);
    InitAddress(addr);
}

void UVReqWrite::InitAddress(const struct sockaddr *addr)
{
    if (addr != NULL)
    {
        if (addr->sa_family == AF_INET6)
            _addr = (struct sockaddr *)Allocator::malloc(sizeof(struct sockaddr_in6));
        else // INET4
            _addr = (struct sockaddr *)Allocator::malloc(sizeof(struct sockaddr_in));

        if (_addr != NULL)
            *_addr = *addr;
    }
}

UVReqWrite::~UVReqWrite()
{
    DEBUG("Object @%p\n", this);
    if (!_bBuffers)
    {
        if (_bCopye && d._data != NULL)
        {
            Allocator::free(d._data); // TODO:
            d._data = NULL;
            d._nsize = 0;
        }
    }
    else
    {
        if (_bCopye && d2._bufs && d2._nbuf > 0)
        {
            for (int i = 0; i < d2._nbuf; ++i)
                Allocator::free(d2._bufs[i]); // TODO:

            Allocator::free(d2._bufs); // TODO:
        }

        d2._bufs = NULL;
        d2._nbuf = 0;
    }

    if (_addr != NULL)
    {
        Allocator::free(_addr); // TODO:
        _addr = NULL;
    }
}

bool UVReqWrite::Start()
{
    auto device = _iohandle.lock();
    if (NULL == _req || NULL == device)
        return false;

    uv_handle_t *uvhandle = device->GetHandle<uv_handle_t>();
    if (NULL == uvhandle)
        return false;

    uv_buf_t *uvbuf = NULL;
    int nbufs = 0;
    uv_buf_t buf;

    if (!_bBuffers)
    {
        buf = uv_buf_init((char *)d._data, d._nsize);
        uvbuf = &buf;
        nbufs = 1;
    }
    else
    {
        if (d2._nbuf <= 0 || NULL == d2._bufs)
            return false;

        uv_buf_t *bufs = (uv_buf_t *)Allocator::malloc(sizeof(uv_buf_t) * d2._nbuf);
        if (NULL == bufs)
            return false;

        for (int i = 0; i < d2._nbuf; ++i)
        {
            void *data = d2._bufs[i];
            int size = *(int *)data;
            bufs[i] = uv_buf_init((char *)((char *)data + sizeof(size)), size);
        }

        uvbuf = bufs;
        nbufs = d2._nbuf;
    }

    if (NULL == uvbuf)
        return false; // TODO: 回收自己

    int type = uvhandle->type;
    if (type == UV_TCP || type == UV_TTY)
    {
        auto other = _other.lock();
        if (NULL == other)
        {
            if (!uv_write(GetReq<uv_write_t>(), (uv_stream_t *)uvhandle, uvbuf, nbufs, __OnWrite))
                return true;
        }
        else
        {
            if (!uv_write2(GetReq<uv_write_t>(), (uv_stream_t *)uvhandle, uvbuf, nbufs, other->GetHandle<uv_stream_t>(), __OnWrite))
                return true;
        }
    }
    else if (type == UV_UDP)
    {
        if (NULL == _addr)
            return false;
        if (!uv_udp_send(GetReq<uv_udp_send_t>(), (uv_udp_t *)uvhandle, uvbuf, nbufs, _addr, __OnWriteUDP))
            return true;
    }

    return false; // TODO: 回收自己
}

void UVReqWrite::OnReq(int status)
{
    DEBUG("\n");
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
