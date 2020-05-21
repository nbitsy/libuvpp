
#ifndef _UVIODEVICE_H_
#define _UVIODEVICE_H_

#include "NetAddress.h"
#include "UVHandle.h"
#include "uv.h"
#include <string>

namespace XSpace
{

class UVLoop;

class UVIODevice : public UVHandle
{
public:
    UVIODevice(const std::weak_ptr<UVLoop>& loop, int flags) : UVHandle(loop), _flags(flags) {}
    ~UVIODevice() {}

    bool Bind(uv_handle_t* handle, const std::string& ip, int port, unsigned int flags = 0);
    bool StartRead();
    bool StopRead();

    bool Write(void* data, int nsize, std::weak_ptr<UVHandle> other = std::weak_ptr<UVHandle>(), const struct sockaddr* addr = NULL);
    bool Write(void* bufs[], int nbuf, std::weak_ptr<UVHandle> other = std::weak_ptr<UVHandle>(), const struct sockaddr* addr = NULL);

    bool TryWrite(void* data, int nsize, const struct sockaddr* addr);
    bool TryWrite(void* bufs[], int nbuf, const struct sockaddr* addr);

    inline const NetAddress& LocalAddress() const { return _local; }
    inline const NetAddress& RemoteAddress() const { return _remote; }
    inline const NetAddress& RemoteServerAddress() const { return _remoteServer; }

    int GetAf() const;
    void SetReuse(bool v);

    // TCP/PIPE
    virtual void OnRead(void* data, int nread) {} // XXX: libuv里一次性最多只能读取64*1024个字节 1.34.3
    // UDP
    virtual void OnRead(void* data, int nread, const struct sockaddr* addr, unsigned int flags) {}
    virtual void OnError(int status) {}

private:
    void GetAddress();
    void GetAddress(int type, NetAddress& address, const struct sockaddr* addr = NULL) const;

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

    // 建立通信之前目标地址信息
    NetAddress _remoteServer;
    // 建立通信后本地地址信息
    NetAddress _local;
    // 建立通信后远程地址信息
    NetAddress _remote;
};

} // namespace XSpace

#endif // _UVIODEVICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
