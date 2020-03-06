
#ifndef UVTCP_H_
#define UVTCP_H_

#include "UVStream.h"
#include "UVTypes.h"
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

    inline const EndPointAddress& LocalAddress() const { return _address; }
    const EndPointAddress RemoteAddress() const;

    inline const UVLoop* GetLoop() const { return _loop; }
    inline int GetAf() const { return _af; }

    UVStream* OnNewConnection();
    void OnAccept(UVStream *client) {}
    void OnAccepted(UVStream *server);
    virtual void OnConnected() {}

    void OnRead(void *data, int nread);
    void OnClose();
    void OnShutdown() {}

private:
    UVLoop* _loop;
    EndPointAddress _address;
    int _af;
    unsigned int _flags;
};

} // namespace XNode

#endif // UVTCP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
