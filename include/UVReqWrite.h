
#ifndef UVREQWRITE_H_
#define UVREQWRITE_H_

#include "UVReq.h"

namespace XNode
{

class UVStream;

class UVReqWrite : public UVReq
{
public:
    /**
     * stream 输出目标流对象
     * other 管道目标流对象
     * data 数据
     * nsize 数据长
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(UVStream* stream, UVStream* other, void* data, int nsize, bool copy = false, bool gc = true);

    /**
     * stream 输出目标流对象
     * other 管道目标流对象
     * bufs 数据缓存对象数组，每个数据包的数据长度在头4个字节中，就是 *(int*)bufs[i] 为这个包的长度，不包括sizeof(int)
     * nbuf 缓存区中数据包个数
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(UVStream* stream, UVStream* other, void* bufs[], int nbuf, bool copy = false, bool gc = true);
    ~UVReqWrite();

    bool Begin();
    void OnReq(int status);

private:
    // 是否复制源数据
    bool _bCopye;
    UVStream* _stream;
    UVStream* _other;

    bool _bBuffers;
    union {
        struct {
            void *_data;
            int _nsize;
        } d;

        struct {
            void **_bufs;
            int _nbuf;
        } d2;
    };
};

} // namespace XNode

#endif // UVREQWRITE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

