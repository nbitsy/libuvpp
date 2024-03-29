
#ifndef _NETSLICESTREAM_H_
#define _NETSLICESTREAM_H_

#include "Config.h"
#include "Stream.h"
#include "UVTcp.h"
#include "Slice.h"

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
    static const int WIRTE_BUFFER_SIZE = 128;
    static const int WRITE_BUFFER_SIZE_MAX = 1024;
#else
    static const int READ_BUFFER_SIZE = 64 * 1024;
    static const int READ_BUFFER_SIZE_MAX = 128 * 1024;
    static const int WIRTE_BUFFER_SIZE = 64 * 1024;
    static const int WRITE_BUFFER_SIZE_MAX = 128 * 1024;
#endif

    static Slice* CreateSlice(int nsize, _OUT int& total);
    // nsize 传入data的长度，成功后返回Slice的长度
    static Slice* MakeSlice(void* data, _IN _OUT int& nsize,
                            MsgID_t MsgID = 0, unsigned char FwdTargetType = 0,
                            unsigned int FwdTarget = 0, unsigned int Target = 0);
    static void ReleaseSlice(Slice* slice);
    static SimpleSlice* CreateSimpleSlice(int nsize, _OUT int& total);
    static SimpleSlice* MakeSimpleSlice(void* data, _IN _OUT int& nsize, MsgID_t MsgID = 0);
    static void ReleaseSlice(SimpleSlice* slice);

public:
    UV_CREATE_HANDLE(NetSliceStream)

public:
    NetSliceStream(const std::weak_ptr<UVLoop>& loop, int flags = AF_UNSPEC);
    ~NetSliceStream();

    std::shared_ptr<UVHandle> OnNewSession() OVERRIDE;
    void OnRead(void* data, int nread) OVERRIDE;

    // 收到一个完整的Slice，SimpleSlice也由这个来处理，函数内部需要识别
    virtual bool RecvedSlice(Slice* slice);
    // 消息头部的标志位处理
    virtual Slice* DealFlags(_NOMODIFY Slice* slice);
    // 消息头部的标志位处理
    virtual SimpleSlice* DealFlags(_NOMODIFY SimpleSlice* slice);
    // 把data组装进一个Slice发送出去
    bool WriteBySlice(void* data, int nsize,
               MsgID_t MsgID = 0, unsigned char FwdTargetType = 0,
               unsigned int FwdTarget = 0, unsigned int Target = 0) OVERWRITE;
    // 发送一个Slice
    inline bool WriteSlice(Slice* slice) { return slice ? UVTcp::Write(slice, slice->Length) : false; }
    // 把data组装成一个SimpleSlice发送出去
    bool WriteBySimpleSlice(void* data, int nsize, MsgID_t MsgID = 0);
    // 发送一个SimpleSlice
    inline bool WriteSimpleSlice(SimpleSlice* slice) { return slice ? UVTcp::Write(slice, slice->Length) : false; }

private:
    MemStream* GetSpliceBuffer(int nread);
    bool HasSpliceSlice() const;
    void ClearReadBrokenBuffer();
    void ClearReadBroken();

private:
    // 包不完整
    bool _readBroken;
    // 拼包时缓存，完整后释放，因为拼包的机会不多，不要占着资源
    void* _readBrokenBuffer;
    // 用于控制拼包缓存
    MemStream* _brokenBufferReader;
};

} // namespace XSpace

#endif // _NETSLICESTREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
