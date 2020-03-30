
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
    UVTcpConnectTimeoutTimer(const std::weak_ptr<UVLoop> &loop, std::weak_ptr<UVHandle> &tcp)
        : UVTimer(loop), _tcp(tcp)
    {
    }
    ~UVTcpConnectTimeoutTimer()
    {
    }

    void Tick(const Timestamp *now)
    {
        auto tcp = _tcp.lock();
        if (NULL == tcp)
        {
            Stop();
            return;
        }

        UVTcp *t = (UVTcp *)tcp.get();
        if (!t->IsConnected())
        {
            t->Reconnect();
        }
        else
        {
            Stop();
            t->Close();
        }
    }

private:
    std::weak_ptr<UVHandle> _tcp;
};

UVTcp::UVTcp(const std::weak_ptr<UVLoop> &loop, int flags)
    : UVStream(loop, flags), _timeoutTimer(NULL), _connected(false), _timeout(0)
{
    if (loop.expired())
        return;

    Init();
    DEBUG("Object @%p\n", this);
}

void UVTcp::Clear()
{
    StopRead();
    ClearData();
    Allocator::Destroy((uv_tcp_t *)_handle);
}

bool UVTcp::Init()
{
    if (_handle != NULL)
    {
        Clear();
        _handle = NULL;
    }

    auto loop = _loop.lock();
    if (NULL == loop)
        return false;

    _handle = (uv_handle_t *)Allocator::Construct<uv_tcp_t>();
    if (NULL == _handle)
        return false;

    if (_handle != NULL)
    {
        uv_tcp_init_ex(loop->GetRawLoop<uv_loop_t>(), (uv_tcp_t *)_handle, _flags);
        uv_handle_set_data(_handle, NULL);
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
    _connector = false;
    return UVIODevice::Bind(GetHandle<uv_handle_t>(), ip, port, flags);
}

void UVTcp::OnAccept(std::weak_ptr<UVHandle> &client)
{
    DEBUG("\n");
}

bool UVTcp::StartConnect(const std::string &ip, int port, int timeout)
{
    DEBUG("\n");
    _connector = true;
    std::weak_ptr<UVHandle> self(shared_from_this());
    std::shared_ptr<UVReqConnect> req = UVReqConnect::Create(self, ip, port);
    if (NULL == req)
        return false;

    if (_timeout == 0)
        _timeout = timeout;
    _local.Ip = ip;
    _local.Port = port;

    if (!req->Start())
        return false;

    StartReconnectTimer();
    return true;
}

void UVTcp::Reconnect()
{
    DEBUG("\n");
    if (_timeout > 0)
    {
        Init();
        StartConnect(_local.Ip, _local.Port); // _timeout);
        SetData(NULL, true);                  // Init把数据清了，这里再重置一下
    }
    else
    {
        StopRead();
        StopReconnectTimer();
        Close();
    }
}

void UVTcp::StartReconnectTimer()
{
    if (NULL == _timeoutTimer && _timeout > 0)
    {
        std::weak_ptr<UVHandle> self(shared_from_this());
        _timeoutTimer = UVTcpConnectTimeoutTimer::Create<UVTcpConnectTimeoutTimer, UVLoop>(GetLoop(), self);
        if (_timeoutTimer != NULL)
            _timeoutTimer->Start(_timeout, _timeout);
    }
}

void UVTcp::StopReconnectTimer()
{
    if (_timeoutTimer != NULL)
    {
        _timeoutTimer->Stop();
        _timeoutTimer = NULL;
    }
}

void UVTcp::OnError(int status)
{
}

void UVTcp::OnErrorAction(int status)
{
    DEBUG("status :%d\n", status);
    _connected = false;

    if (IsConnector() && _timeout > 0)
    {
        if (NULL == _timeoutTimer)
        {
            std::weak_ptr<UVHandle> self(shared_from_this());
            _timeoutTimer = UVTcpConnectTimeoutTimer::Create<UVTcpConnectTimeoutTimer>(GetLoop(), self);
            if (_timeoutTimer != NULL)
                _timeoutTimer->Start(_timeout, _timeout);
        }
    }
    else
    {
        Close();
    }
}

std::shared_ptr<UVHandle> UVTcp::OnNewConnection()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return UVTcp::CreateStrong(GetLoop());
}

void UVTcp::OnConnected()
{
    DEBUG("\n");
}

void UVTcp::OnConnectedAction()
{
    DEBUG("\n");
    _connected = true;

    if (_timeoutTimer != NULL)
    {
        _timeoutTimer->Stop();
        _timeoutTimer = NULL;
    }

    InitAddress();
    StartRead();

    DEBUG("%s Connected To %s\n", LocalAddress().ToString().c_str(), RemoteAddress().ToString().c_str());
}

void UVTcp::OnRead(void *data, int nread)
{
    DEBUG("RECV FROM %s nread: %d\n", RemoteAddress().ToString().c_str(), nread)
    if (nread < 0)
    {
        OnError(-1);
        OnErrorAction(-1);
    }
}

void UVTcp::OnAccepted(std::weak_ptr<UVHandle> &server)
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

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
