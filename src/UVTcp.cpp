
#include "UVTcp.h"
#include "UVReqConnect.h"
#include "UVLoop.h"

namespace XNode
{

UVTcp::UVTcp(UVLoop *loop, int flags) : UVStream(loop, flags)
{
    if (NULL == _loop)
        return;

    _handle = (uv_handle_t *)_loop->Construct<uv_tcp_t>();
    if (_handle != NULL)
    {
        uv_tcp_init_ex(_loop->GetRawLoop<uv_loop_t>(), (uv_tcp_t *)_handle, flags);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVTcp::~UVTcp()
{
    DEBUG("Object @%p\n", this);
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
    auto tcp = new UVTcp(GetLoop());
    return tcp;
}

void UVTcp::OnConnected()
{
    DEBUG("%s Connected To %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
    const char *msg = "hello";
    Write((void *)msg, 5);
}

void UVTcp::OnRead(void *data, int nread)
{
    DEBUG("RECV FROM %s\n", RemoteAddress().ToString().c_str())
    DEBUG("data: %s, len: %d\n", (char*)data, nread);
    Write((char*)"BACK", 4);
}

void UVTcp::OnAccepted(UVStream *server)
{
    DEBUG("%s => %s\n", RemoteAddress().ToString().c_str(), LocalAddress().ToString().c_str());
}

/**
 * 对于一个UVHandle对象来说，因为只有被关闭后才可以释放
*/
void UVTcp::OnClosed()
{
    DEBUG("%s %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
}

void UVTcp::OnShutdown()
{
    DEBUG("%s %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
}

void UVTcp::Release()
{
    DEBUG("\n");
    if (NULL == _handle)
        return;

    auto loop = GetLoop();
    if (NULL == loop)
        return;
    
    ClearData();
    loop->Destroy((uv_tcp_t*)_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
