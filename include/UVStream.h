
#ifndef _UVSTREAM_H_
#define _UVSTREAM_H_

#include "uv.h"
#include "UVIODevice.h"

namespace XNode
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
    UVStream(UVLoop* loop, int flags, EUVStreamType type = EUVS_RW);
    ~UVStream();

    bool Listen(int backlog = 10000);

    inline bool Write(void *data, int nsize) { return UVIODevice::Write(data, nsize, NULL, NULL); }
    /**
     * bufs里的每一个数据里的前sizeof(int)个字节为包的长度
    */
    inline bool Write(void* bufs[], int nbuf) { return UVIODevice::Write(bufs, nbuf, NULL, NULL); }
    inline bool Write2(void *data, int nsize, UVStream *other) { return UVIODevice::Write(data, nsize, other, NULL); }
    inline bool Write2(void *bufs[], int nbuf, UVStream *other) { return UVIODevice::Write(bufs, nbuf, other, NULL); }

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
    virtual UVStream* OnNewConnection() = 0;
    /**
     * 这个流如果接收了一个连接后调用OnAccept
    */
    virtual void OnAccept(UVStream *client) = 0;
    /**
     * 服务器建立了一个新的连接后，创建了一个UVStream，在接受这个连接后会调用新建UVStream对象
     * 的OnAccepted来通知新建的UVStream已经被接受了
    */
    virtual void OnAccepted(UVStream *server) = 0;

    virtual void OnClosed() {}
    virtual void OnShutdown() = 0;

public:
    bool Accept(UVStream *client);

private:
    EUVStreamType _type;
};

} // namespace XNode

#endif // _UVSTREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
