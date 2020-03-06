
#include "UVTcp.h"
#include "UVReqConnect.h"

namespace XNode
{

UVTcp::UVTcp(UVLoop *loop) : _loop(loop), _af(AF_MAX), _flags(0)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_tcp_t));
    if (_handle != NULL)
        uv_tcp_init(_loop->GetHandle<uv_loop_t>(), (uv_tcp_t *)_handle);

    SetData(NULL);
}

UVTcp::~UVTcp()
{
}

void UVTcp::SetDelay(bool delay)
{
    uv_tcp_nodelay(GetHandle<uv_tcp_t>(), !delay);
}

void UVTcp::KeepAlive(bool v, unsigned int delay)
{
    uv_tcp_keepalive(GetHandle<uv_tcp_t>(), v, delay);
}

bool UVTcp::Bind(const std::string &ip, int port, int af, unsigned int flags)
{
    struct sockaddr *addr = NULL;
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

    if (!uv_tcp_bind(GetHandle<uv_tcp_t>(), addr, flags))
    {
        _af = af;
        _flags = flags;
        return true;
    }

    return false;
}

void UVTcp::OnAccept(UVStream *client)
{
    if (NULL == client)
        return;

    ((UVTcp *)client)->InitAddress();
}

void UVTcp::InitAddress()
{
    struct sockaddr_in *addr = NULL;
    int len = sizeof(_addr);
    if (_af == AF_INET6)
    {
        addr = (struct sockaddr_in *)&_addr6;
        len = sizeof(_addr6);
    }
    else
        addr = (struct sockaddr_in *)&_addr;

    uv_tcp_getsockname(GetHandle<uv_tcp_t>(), (struct sockaddr *)addr, &len);

    len = sizeof(_addr);
    if (_af == AF_INET6)
    {
        addr = (struct sockaddr_in *)&_addr6Peer;
        len = sizeof(_addr6);
    }
    else
        addr = (struct sockaddr_in *)&_addrPeer;

    uv_tcp_getpeername(GetHandle<uv_tcp_t>(), (struct sockaddr *)addr, &len);
}

bool UVTcp::BeginConnect(const std::string &ip, int port)
{
    UVReqConnect *req = new UVReqConnect(this, ip, port); // TODO:
    if (req != NULL)
        return req->Begin();

    return false;
}

void UVTcp::GetAddress(int type, EndPointAddress &address) const
{
    char ip[128] = {
        0,
    };
    const struct sockaddr_in *addr = NULL;
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

    if (addr->sin_family == AF_INET)
        uv_ip4_name(addr, ip, sizeof(ip));
    else if (_addr.sin_family == AF_INET6)
        uv_ip6_name((const struct sockaddr_in6 *)addr, ip, sizeof(ip));

    address._ip = ip;
    address._port = ntohs(addr->sin_port);
}

const EndPointAddress UVTcp::LocalAddress() const
{
    EndPointAddress address;
    GetAddress(0, address);
    return address;
}

const EndPointAddress UVTcp::RemoteAddress() const
{
    EndPointAddress address;
    GetAddress(1, address);
    return address;
}

UVStream *UVTcp::OnNewConnection()
{
    std::cout << __FUNCTION__ << std::endl;
    auto tcp = new UVTcp((UVLoop *)GetLoop());
    return tcp;
}

void UVTcp::OnRead(void *data, int nread)
{
    std::cout << __FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
    std::cout << "data: " << (char *)data << " len: " << nread << std::endl;
}

void UVTcp::OnAccepted(UVStream *server)
{
    std::cout << __FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
}

void UVTcp::OnClosed()
{
    std::cout << __FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
