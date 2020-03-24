
#ifndef _SLICE_H_
#define _SLICE_H_

#include "Config.h"

namespace XSpace
{

/**
 * 对于一个Slice对象来说，本质上是一个变长对象，但分配对象内存时，需要把分片的长度加上去
*/

const unsigned char SLICE_FWD = ((unsigned char)0x01 << 0); // 不要解包，只转发
const unsigned char SLICE_ZIP = ((unsigned char)0x01 << 1); // 是否压缩
// 加密方式，可以实现多重加密，但是检测循序为从上到下，所以加密的时候需要从下到上的顺序进行
const unsigned char SLICE_DES = ((unsigned char)0x01 << 2); // DES加密
const unsigned char SLICE_RC5 = ((unsigned char)0x01 << 3); // RC5密码
// 其他
const unsigned char SLICE_RWD_4 = ((unsigned char)0x01 << 4);
const unsigned char SLICE_RWD_5 = ((unsigned char)0x01 << 5);
const unsigned char SLICE_RWD_6 = ((unsigned char)0x01 << 6);
const unsigned char SLICE_RWD_7 = ((unsigned char)0x01 << 7);

#define SLICE_FLAG_GET(T) (Flags & SLICE_##T)
#define SLICE_FLAG_SET(T, v)     \
    {                            \
        if ((v))                 \
            Flags |= SLICE_##T;  \
        else                     \
            Flags &= ~SLICE_##T; \
    }

struct Slice
{
    Slice() {}
    ~Slice() {}

    inline int HeadLength() const { return sizeof(Slice); }
    inline void *Body() { return &_all + 1; }
    inline int BodyLength() const { return Length >= sizeof(Slice) ? Length - sizeof(Slice) : 0; }

    inline void SetFwd(bool v) { SLICE_FLAG_SET(FWD, v); }
    inline bool IsFwd() const { return SLICE_FLAG_GET(FWD); }
    inline void SetZip(bool v) { SLICE_FLAG_SET(ZIP, v); }
    inline bool IsZipped() const { return SLICE_FLAG_GET(ZIP); }
    inline void SetDES(bool v) { SLICE_FLAG_SET(DES, v); }
    inline bool IsDES() const { return SLICE_FLAG_GET(DES); }
    inline void SetRC5(bool v) { SLICE_FLAG_SET(RC5, v); }
    inline bool IsRC5() const { return SLICE_FLAG_GET(RC5); }

    union {
        struct
        {
#ifdef LITTLE_ENDIAN
            // 这个长度包括Slice的大小，也就是说真实数据的大小为包体长-sizeof(Slice)
            int Length : 24; // 低
            int Flags : 8;   // 高
#else
            int Flags : 4;   // 高
            int Length : 28; // 低
#endif
        };
        int _all;
    };
};

} // namespace XSpace

#endif // _SLICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
