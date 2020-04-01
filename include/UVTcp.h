
#ifndef _UVTCP_H_
#define _UVTCP_H_

#include "NetAddress.h"
#include "UVStream.h"
#include <iostream>

namespace XSpace
{

class UVTimer;
class UVLoop;

class UVTcp : public UVStream
{
public:
    UV_CREATE_HANDLE(UVTcp)

public:
    ~UVTcp();

    bool Bind(const std::string &ip, int port, unsigned int flags = 0) OVERWRITE;

    inline void SetNoDelay() { SetDelay(false); }
    void SetDelay(bool delay);
    void KeepAlive(bool v, unsigned int delay);

    // timeout : 断线重连间隔或连接超时
    bool StartConnect(const std::string &ip, int port, int timeout = 0);

    std::shared_ptr<UVHandle> OnNewConnection() OVERRIDE;
    void OnAccepted(std::weak_ptr<UVHandle> &server) OVERRIDE;
    void OnAccept(std::weak_ptr<UVHandle> &client) OVERRIDE;

    virtual void OnConnected();
    void OnError(int status) OVERRIDE;

    void Reconnect();

    inline bool IsConnected() const { return _connected; }
    inline bool IsConnector() const { return _connector; }
    inline bool IsServer() const { return !_connector; }
    inline bool NeedReconnect() const { return !IsConnected() && _timeout > 0; }

    void OnRead(void *data, int nread) OVERRIDE;
    void OnClosed() OVERRIDE;
    void OnShutdown() OVERRIDE;

    void OnConnectedAction();
    bool OnErrorAction(int status);

protected:
    UVTcp(const std::weak_ptr<UVLoop> &loop, int flags = AF_UNSPEC);

private:
    bool Init();
    void StartReconnectTimer();
    void StopReconnectTimer();

private:
    std::shared_ptr<UVTimer> _timeoutTimer;
    bool _connected;
    bool _connector;
    int _timeout;
    NetAddress _local;
};

} // namespace XSpace

#endif // _UVTCP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
