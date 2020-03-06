
#ifndef UVTCP_H_
#define UVTCP_H_

#include "UVStream.h"
#include "EndPointAddress.h"
#include <iostream>

namespace XNode
{

class UVLoop;

class UVTcp : public UVStream
{
public:
    UVTcp(UVLoop* loop);
    ~UVTcp();

    inline void SetNoDelay() { SetDelay(false); }
    void SetDelay(bool delay);
    void KeepAlive(bool v, unsigned int delay);

    bool Bind(const std::string &ip, int port, int af = AF_INET, unsigned int flags = 0);
    bool BeginConnect(const std::string& ip, int port);

    const EndPointAddress LocalAddress() const;
    const EndPointAddress RemoteAddress() const;

    inline const UVLoop* GetLoop() const { return _loop; }
    inline int GetAf() const { return _af; }

    UVStream* OnNewConnection();
    void OnAccepted(UVStream *server);
    void OnAccept(UVStream *client);
    virtual void OnConnected() {}

    void OnRead(void *data, int nread);
    void OnClosed();
    void OnShutdown() {}

private:
    void InitAddress();
    void GetAddress(int type, EndPointAddress& address) const;

private:
    UVLoop* _loop;
    int _af;
    unsigned int _flags;
    // local
    union {
        struct sockaddr_in _addr;
        struct sockaddr_in6 _addr6;
    };
    // peer
    union {
        struct sockaddr_in _addrPeer;
        struct sockaddr_in6 _addr6Peer;
    };
};

} // namespace XNode

#endif // UVTCP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
