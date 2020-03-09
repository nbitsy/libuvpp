
#ifndef UVTCP_H_
#define UVTCP_H_

#include "EndPointAddress.h"
#include "UVStream.h"
#include <iostream>

namespace XNode
{

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
    bool Bind(const std::string &ip, int port, unsigned int flags = 0);

    inline void SetNoDelay() { SetDelay(false); }
    void SetDelay(bool delay);
    void KeepAlive(bool v, unsigned int delay);

    bool StartConnect(const std::string &ip, int port);

    inline const UVLoop *GetLoop() const { return _loop; }

    UVStream *OnNewConnection();
    void OnAccepted(UVStream *server);
    void OnAccept(UVStream *client);
    virtual void OnConnected();

    void OnRead(void *data, int nread);
    void OnClosed();
    void OnShutdown();

private:
    /**
     * 构造函数和析构函数放这里说明不允许在栈里去构造一个UVTcp对象
    */
    UVTcp(UVLoop *loop, int flags = AF_UNSPEC);
    ~UVTcp();
};

} // namespace XNode

#endif // UVTCP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
