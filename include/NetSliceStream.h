
#ifndef _NETSLICESTREAM_H_
#define _NETSLICESTREAM_H_

#include "Stream.h"
#include "UVTcp.h"

/**
 * 对于一个SliceStream来说，本质上是一个UVTcp，只不过把收到数据缓存起来，而且是分片缓存
*/
namespace XSpace
{

class MemStream;

class NetSliceStream : public UVTcp
{
public:
#ifdef _DEBUG
    static const int READ_BUFFER_SIZE = 16;
    static const int READ_BUFFER_SIZE_MAX = 32;
#else
    static const int READ_BUFFER_SIZE = 64 * 1024;
    static const int READ_BUFFER_SIZE_MAX = 256 * 1024;
#endif

public:
    NetSliceStream(UVLoop *loop, int flags = AF_UNSPEC);
    ~NetSliceStream();

    UVStream *OnNewConnection();
    void OnAccepted(UVStream *server);
    void OnConnected();

    void OnRead(void *data, int nread);
    void OnClosed();
    void OnShutdown();
    void Release();

    // 收到一个完整的Slice
    virtual void PushSlice(void *data, int nsize, unsigned int flags = 0);

private:
    MemStream *GetSpliceBuffer(int nread);
    bool HasSpliceSlice() const;
    void ClearReadBrokenBuffer();
    void ClearReadBroken();

private:
    // 包不完整
    bool _readBroken;
    // 拼包时缓存，完整后释放，因为拼包的机会不多，不要占着资源
    void *_readBrokenBuffer;
    // 用于控制拼包缓存
    MemStream *_readBrokenBufferStream;
};

} // namespace XSpace

#endif // _NETSLICESTREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
