
#ifndef UVSTREAM_H_
#define UVSTREAM_H_

#include "UVHandle.h"
#include "uv.h"

namespace XNode
{

enum EUVStreamType
{
    EUVS_READ = 1,
    EUVS_WRITE = 2,
    EUVS_RW = EUVS_READ | EUVS_WRITE,
};

class UVStream : public UVHandle
{
public:
    explicit UVStream(EUVStreamType type = EUVS_RW);
    ~UVStream();

    bool Listen(int backlog = 10000);
    bool StopRead();

    inline bool Write(void *data, int nsize) { return Write2(data, nsize, NULL); }
    /**
     * bufs里的每一个数据里的前sizeof(int)个字节为包的长度
    */
    bool Write(void* bufs[], int nbuf) { return Write2(bufs, nbuf, NULL); }
    bool Write2(void *data, int nsize, UVStream *other);
    bool Write2(void *bufs[], int nbuf, UVStream *other);

    bool TryWrite(void* data, int nsize);
    bool TryWrite(void* bufs[], int nbuf);

    bool IsReadable() const;
    bool IsWritable() const;

    size_t GetWriteQueueSize() const;

    bool Shutdown();
    void SetBlocking(bool v = true);

    virtual UVStream* OnNewConnection() = 0;
    virtual void OnRead(void *data, int nread)  = 0;
    virtual void OnAccept(UVStream *client) = 0;
    virtual void OnAccepted(UVStream *server) = 0;

    virtual void OnClosed() {}
    virtual void OnShutdown() {}

public:
    bool Accept(UVStream *client);

private:
    bool BeginRead();

private:
    EUVStreamType _type;
};

} // namespace XNode

#endif // UVSTREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
