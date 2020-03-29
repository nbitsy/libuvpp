
#ifndef _UVUDP_H_
#define _UVUDP_H_

#include "NetAddress.h"
#include "UVIODevice.h"

namespace XSpace
{

class UVUdp : public UVIODevice
{
public:
    UV_CREATE_HANDLE(UVUdp)

public:
    ~UVUdp();
    bool Bind(const std::string &ip, int port, unsigned int flags = 0);
    bool Connect(const std::string &ip, int port);

    inline bool Write(void *data, int nsize, const struct sockaddr *addr)
    {
        std::weak_ptr<UVHandle> other;
        return UVIODevice::Write(data, nsize, other, addr);
    }
    /**
     * bufs里的每一个数据里的前sizeof(int)个字节为包的长度
    */
    inline bool Write(void *bufs[], int nbuf, const struct sockaddr *addr)
    {
        std::weak_ptr<UVHandle> other;
        return UVIODevice::Write(bufs, nbuf, other, addr);
    }

    inline bool TryWrite(void *data, int nsize, const struct sockaddr *addr)
    {
        return UVIODevice::TryWrite(data, nsize, addr);
    }
    inline bool TryWrite(void *bufs[], int nbuf, const struct sockaddr *addr)
    {
        return UVIODevice::TryWrite(bufs, nbuf, addr);
    }

    bool SetBroadcast(bool v);
    bool SetTTL(int ttl);

    size_t SendQueueSize() const;
    size_t SendQueueCount() const;

    void OnRead(void *data, int nread, const struct sockaddr *addr, unsigned int flags) OVERRIDE;
    void OnClosed() OVERRIDE;

protected:
    UVUdp(std::weak_ptr<UVLoop> &loop, int flags = AF_UNSPEC);
};

} // namespace XSpace

#endif // _UVUDP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
