
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
    static UVTcp *Create(UVLoop *loop, int flags = AF_UNSPEC)
    {
        return new UVTcp(loop, flags); // TODO:
    }

    static void Destroy(UVTcp *tcp)
    {
        if (NULL == tcp)
            return;
        tcp->Close();
    }

public:
    /**
     * 构造函数和析构函数放这里说明不允许在栈里去构造一个UVTcp对象
    */
    UVTcp(UVLoop *loop, int flags = AF_UNSPEC);
    ~UVTcp();

    bool Init();
    void Clear();

    bool Bind(const std::string &ip, int port, unsigned int flags = 0);

    inline void SetNoDelay() { SetDelay(false); }
    void SetDelay(bool delay);
    void KeepAlive(bool v, unsigned int delay);

    // timeout : 断线重连间隔或连接超时
    bool StartConnect(const std::string &ip, int port, int timeout = 0);

    UVStream *OnNewConnection();
    void OnAccepted(UVStream *server);
    void OnAccept(UVStream *client);
    virtual void OnConnected();
    void OnError(int status);
    void Reconnect();

    inline bool IsConnected() const { return _connected; }

    void OnRead(void *data, int nread);
    void OnClosed();
    void OnShutdown();
    void Release();

private:
    UVTimer* _timeoutTimer;
    bool _connected;
    int _timeout;
    NetAddress _local;
};

} // namespace XSpace

#endif // _UVTCP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
