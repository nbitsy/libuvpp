
#ifndef UVIODEVICE_H_
#define UVIODEVICE_H_

#include "EndPointAddress.h"
#include "UVHandle.h"
#include "uv.h"
#include <string>

namespace XNode
{

class UVLoop;

class UVIODevice : public UVHandle
{
public:
    UVIODevice(UVLoop *loop, int flags) : UVHandle(loop), _flags(flags) {}
    ~UVIODevice() {}

    bool Bind(uv_handle_t *handle, const std::string &ip, int port, unsigned int flags = 0);
    bool StartRead();
    bool StopRead();

    bool Write(void *data, int nsize, UVIODevice *other = NULL, const struct sockaddr *addr = NULL);
    bool Write(void *bufs[], int nbuf, UVIODevice *other = NULL, const struct sockaddr *addr = NULL);

    bool TryWrite(void *data, int nsize, const struct sockaddr *addr);
    bool TryWrite(void *bufs[], int nbuf, const struct sockaddr *addr);

    const EndPointAddress LocalAddress() const;
    const EndPointAddress RemoteAddress(const struct sockaddr *remote = NULL) const;
    int GetAf() const;

    // TCP/PIPE
    virtual void OnRead(void *data, int nread) {}
    // UDP
    virtual void OnRead(void *data, int nread, const struct sockaddr *addr, unsigned int flags) {}

private:
    void GetAddress(int type, EndPointAddress &address, const struct sockaddr *addr = NULL) const;

protected:
    void InitAddress();

protected:
    int _flags;
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

#endif // UVIODEVICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
