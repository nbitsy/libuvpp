
#include "UVTcp.h"
#include "UVReqConnect.h"

namespace XNode
{

UVTcp::UVTcp(UVLoop *loop, int flags) : UVStream(loop, flags)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_tcp_t)); // TODO:
    if (_handle != NULL)
    {
        std::cout << "Malloc @" << (void*)_handle << std::endl;
        uv_tcp_init_ex(_loop->GetLoop<uv_loop_t>(), (uv_tcp_t *)_handle, flags);
    }

    SetData(NULL);
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVTcp::~UVTcp()
{
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

void UVTcp::SetDelay(bool delay)
{
    uv_tcp_nodelay(GetHandle<uv_tcp_t>(), !delay);
}

void UVTcp::KeepAlive(bool v, unsigned int delay)
{
    uv_tcp_keepalive(GetHandle<uv_tcp_t>(), v, delay);
}

bool UVTcp::Bind(const std::string &ip, int port, unsigned int flags)
{
    return UVIODevice::Bind(GetHandle<uv_handle_t>(), ip, port, flags);
}

void UVTcp::OnAccept(UVStream *client)
{
    if (NULL == client)
        return;

    ((UVTcp *)client)->InitAddress();
}

bool UVTcp::StartConnect(const std::string &ip, int port)
{
    UVReqConnect *req = new UVReqConnect(this, ip, port); // TODO:
    if (req != NULL)
        return req->Start();

    return false;
}

UVStream *UVTcp::OnNewConnection()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto tcp = new UVTcp((UVLoop *)GetLoop());
    return tcp;
}

void UVTcp::OnConnected()
{
    std::cout << __PRETTY_FUNCTION__ << " " << LocalAddress().ToString() << " Connected To => " << RemoteAddress().ToString() << std::endl;
    const char *msg = "hello";
    Write((void *)msg, 5);
}

void UVTcp::OnRead(void *data, int nread)
{
    std::cout << __PRETTY_FUNCTION__ << " RECV FROM " << RemoteAddress().ToString() << std::endl;
    std::cout << "data: " << (char *)data << " len: " << nread << std::endl;
}

void UVTcp::OnAccepted(UVStream *server)
{
    std::cout << __PRETTY_FUNCTION__ << " " << RemoteAddress().ToString() << " => " << LocalAddress().ToString() << std::endl;
}

/**
 * 对于一个UVHandle对象来说，因为只有被关闭后才可以释放
*/
void UVTcp::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
    delete this;
}

void UVTcp::OnShutdown()
{
    std::cout << __PRETTY_FUNCTION__ << " " << LocalAddress().ToString() << " => " << RemoteAddress().ToString() << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
