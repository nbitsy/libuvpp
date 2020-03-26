
#include "UVTcp.h"
#include "Allocator.h"
#include "UVLoop.h"
#include "UVReqConnect.h"
#include "UVTimer.h"

namespace XSpace
{

class UVTcpConnectTimeoutTimer : public UVTimer
{
public:
    UVTcpConnectTimeoutTimer(UVLoop *loop, UVTcp *tcp)
        : UVTimer(loop), _tcp(tcp)
    {
    }
    ~UVTcpConnectTimeoutTimer()
    {
    }

    void Tick(const Timestamp *now)
    {
        if (!_tcp->IsConnected())
        {
            _tcp->Reconnect();
        }
        else
        {
            Stop();
            _tcp->Close();
        }
    }

private:
    UVTcp *_tcp;
};

UVTcp::UVTcp(UVLoop *loop, int flags)
    : UVStream(loop, flags), _timeoutTimer(NULL), _connected(false), _timeout(0)
{
    if (NULL == _loop)
        return;
    Init();
    DEBUG("Object @%p\n", this);
}

void UVTcp::Clear()
{
    auto loop = GetLoop();
    if (NULL == loop)
        return;

    ClearData();
    loop->Destroy((uv_tcp_t *)_handle);
}

bool UVTcp::Init()
{
    if (_handle != NULL)
    {
        Clear();
        _handle = NULL;
    }

    _handle = (uv_handle_t *)_loop->Construct<uv_tcp_t>();
    if (NULL == _handle)
        return false;

    if (_handle != NULL)
    {
        uv_tcp_init_ex(_loop->GetRawLoop<uv_loop_t>(), (uv_tcp_t *)_handle, _flags);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }

    return true;
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

bool UVTcp::StartConnect(const std::string &ip, int port, int timeout)
{
    DEBUG("\n");
    UVReqConnect *req = Allocator::Construct<UVReqConnect>(this, ip, port);
    if (NULL == req)
        return false;

    _timeout = timeout;
    _local.Ip = ip;
    _local.Port = port;

    if (!req->Start())
        return false;

    if (NULL == _timeoutTimer)
    {
        _timeoutTimer = Allocator::Construct<UVTcpConnectTimeoutTimer>(GetLoop(), this);
        if (_timeoutTimer != NULL)
        _timeoutTimer->Start(_timeout, _timeout);
    }

    return true;
}

void UVTcp::Reconnect()
{
    DEBUG("\n");
    if (_timeout > 0)
    {
        Init();
        StartConnect(_local.Ip, _local.Port, _timeout);
        StartRead();
    }
    else
    {
        if (_timeoutTimer != NULL)
        {
            _timeoutTimer->Stop();
            _timeoutTimer = NULL;
        }
        Close();
    }
}

void UVTcp::OnError(int status)
{
    DEBUG("\n");
    _connected = false;
    if (_timeout > 0)
    {
        if (NULL == _timeoutTimer)
        {
            _timeoutTimer = Allocator::Construct<UVTcpConnectTimeoutTimer>(GetLoop(), this);
            if (_timeoutTimer != NULL)
                _timeoutTimer->Start(_timeout);
        }
    }
    else
    {
        Close();
    }
}

UVStream *UVTcp::OnNewConnection()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto tcp = Allocator::Construct<UVTcp>(GetLoop());
    return tcp;
}

void UVTcp::OnConnected()
{
    DEBUG("\n");
    _connected = true;

    if (_timeoutTimer != NULL)
    {
        _timeoutTimer->Stop();
        _timeoutTimer = NULL;
    }

    DEBUG("%s Connected To %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
    const char *msg = "hello";
    Write((void *)msg, 5);
}

void UVTcp::OnRead(void *data, int nread)
{
    if (nread < 0)
        OnError(-1);

    DEBUG("RECV FROM %s\n", RemoteAddress().ToString().c_str())
    ((char *)data)[nread - 1] = '\0';
    DEBUG("data: %s, len: %d\n", (char *)data, nread);
    Write((char *)"BACK", 4);
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
    loop->Destroy((uv_tcp_t *)_handle);
    if (GetGC())
        Allocator::Destroy(this);

    _handle = NULL;
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
