
#include "UVUdp.h"

namespace XNode
{

UVUdp::UVUdp(UVLoop *loop, int flags) : UVIODevice(loop, flags)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_udp_t)); // TODO:
    if (_handle != NULL)
    {
        std::cout << "Malloc @" << (void*)_handle << std::endl;
        uv_udp_init_ex(_loop->GetLoop<uv_loop_t>(), (uv_udp_t *)_handle, flags);
    }

    SetData(NULL);
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVUdp::~UVUdp()
{
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

bool UVUdp::Bind(const std::string &ip, int port, unsigned int flags)
{
    if (NULL == _handle)
        return false;
    return UVIODevice::Bind(GetHandle<uv_handle_t>(), ip, port, flags);
}

bool UVUdp::Connect(const std::string &ip, int port)
{
    if (NULL == _handle)
        return false;

    int af = GetAf();
    struct sockaddr *addr = NULL;
    union {
        struct sockaddr_in _addr;
        struct sockaddr_in6 _addr6;
    };

    if (af == AF_INET6)
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

    if (uv_udp_connect(GetHandle<uv_udp_t>(), addr) == 0)
    {
        InitAddress();
        return true;
    }

    return false;
}

bool UVUdp::SetBroadcast(bool v)
{
    if (NULL == _handle)
        return false;

    return uv_udp_set_broadcast(GetHandle<uv_udp_t>(), v) == 0;
}

bool UVUdp::SetTTL(int ttl)
{
    if (NULL == _handle)
        return false;

    return uv_udp_set_ttl(GetHandle<uv_udp_t>(), ttl) == 0;
}

size_t UVUdp::SendQueueSize() const

{
    if (NULL == _handle)
        return 0;

    return uv_udp_get_send_queue_size(GetHandle<uv_udp_t>());
}

size_t UVUdp::SendQueueCount() const
{
    if (NULL == _handle)
        return 0;

    return uv_udp_get_send_queue_count(GetHandle<uv_udp_t>());
}

// test
void UVUdp::OnRead(void *data, int nread, const struct sockaddr *addr, unsigned int flags)
{
    std::cout << __PRETTY_FUNCTION__ << " RECV FROM " << RemoteAddress(addr).ToString() << std::endl;
    std::cout << "data: " << (char *)data << " len: " << nread << " flags: " << flags << std::endl;
}

/**
 * 对于一个UVHandle对象来说，因为只有被关闭后才可以释放
*/
void UVUdp::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
    delete this; // TODO: 
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
