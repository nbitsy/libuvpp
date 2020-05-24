
#ifndef _UVSTREAM_H_
#define _UVSTREAM_H_

#include "UVIODevice.h"
#include "uv.h"

namespace XSpace
{

class UVLoop;

enum EUVStreamType
{
    EUVS_READ = 1,
    EUVS_WRITE = 2,
    EUVS_RW = EUVS_READ | EUVS_WRITE,
};

class UVStream : public UVIODevice
{
public:
    UVStream(const std::weak_ptr<UVLoop>& loop, int flags, EUVStreamType type = EUVS_RW);
    ~UVStream();

    bool Listen(int backlog = 10000);

    inline bool Write(void* data, int nsize) { return UVIODevice::Write(data, nsize, std::weak_ptr<UVHandle>(), NULL); }
    /**
     * bufs里的每一个数据里的前sizeof(int)个字节为包的长度
    */
    inline bool Write(void* bufs[], int nbuf) { return UVIODevice::Write(bufs, nbuf, std::weak_ptr<UVHandle>(), NULL); }
    inline bool Write2(void* data, int nsize, std::weak_ptr<UVHandle>& other) { return UVIODevice::Write(data, nsize, other, NULL); }
    inline bool Write2(void* bufs[], int nbuf, std::weak_ptr<UVHandle>& other) { return UVIODevice::Write(bufs, nbuf, other, NULL); }

    inline bool TryWrite(void* data, int nsize) { return UVIODevice::TryWrite(data, nsize, NULL); }
    inline bool TryWrite(void* bufs[], int nbuf) { return UVIODevice::TryWrite(bufs, nbuf, NULL); }

    bool IsReadable() const;
    bool IsWritable() const;

    size_t GetWriteQueueSize() const;

    bool Shutdown();
    void SetBlocking(bool v = true);

    /**
     * 作为一个服务器，如果有新的连接进来后，需要创建一个新的UVStream来处理这个连接
    */
    virtual std::shared_ptr<UVHandle> OnNewSession() = 0;
    /**
     * 这个流如果接收了一个连接后调用OnAccept
    */
    virtual void OnAccept(std::weak_ptr<UVHandle>& client) = 0;
    /**
     * 服务器建立了一个新的连接后，创建了一个UVStream，在接受这个连接后会调用新建UVStream对象
     * 的OnAccepted来通知新建的UVStream已经被接受了
    */
    virtual void OnAccepted(std::weak_ptr<UVHandle>& server) = 0;

    virtual void OnClosed() OVERRIDE {}
    virtual void OnShutdown() = 0;

public:
    bool Accept(std::weak_ptr<UVHandle>& client);

private:
    EUVStreamType _type;
};

} // namespace XSpace

#endif // _UVSTREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
