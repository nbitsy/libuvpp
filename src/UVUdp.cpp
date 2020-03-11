
#include "UVUdp.h"
#include "UVLoop.h"

namespace XNode
{

UVUdp::UVUdp(UVLoop *loop, int flags) : UVIODevice(loop, flags)
{
    if (NULL == _loop)
        return;

    _handle = (uv_handle_t *)_loop->Construct<uv_udp_t>();
    if (_handle != NULL)
    {
        uv_udp_init_ex(_loop->GetRawLoop<uv_loop_t>(), (uv_udp_t *)_handle, flags);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVUdp::~UVUdp()
{
    DEBUG("Object @%p\n", this);
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
    DEBUG("RECV FROM %s\n", RemoteAddress().ToString().c_str());
    ((char*)data)[nread-1] = '\0';
    DEBUG("data: %s len: %d\n", (char*)data, nread);
}

/**
 * 对于一个UVHandle对象来说，因为只有被关闭后才可以释放
*/
void UVUdp::OnClosed()
{
    DEBUG("RECV FROM %s => %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
}

void UVUdp::Release()
{
    DEBUG("\n");
    if (NULL == _handle)
        return;

    auto loop = GetLoop();
    if (NULL == loop)
        return;
    
    ClearData();
    loop->Destroy((uv_udp_t*)_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
