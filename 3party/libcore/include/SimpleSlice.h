
#ifndef _SIMPLESLICE_H_
#define _SIMPLESLICE_H_

#include "Allocator.h"
#include "Config.h"
#include "CoreConfig.h"

namespace XSpace
{

class Slice;

/**
 * SimpleSlice是最低层的通信包结构，很简单，只有一个长度，所有数据跟在长度后面，而Msg是基于
 * SimpleSlice的实现，对于一个SimpleSlice的数据包来说，他的包内容可以是Msg，或者其他种类的Msg。
 * 基于TCP协议的连接收到的一个SimpleSlice可能是不完整的，所以需要进行拼包处理。但是基于UDP
 * 协议的连接一个SimpleSlice就是一个包，因为UDP不分包。
*/

/**
 * 对于一个SimpleSlice对象来说，本质上是一个变长对象，但分配对象内存时，需要把分片的长度加上去
*/

const unsigned char SLICE_FWD = ((unsigned char)0x01 << 0); // 不要解包，只转发
const unsigned char SLICE_ZIP = ((unsigned char)0x01 << 1); // 是否压缩
// 加密方式，可以实现多重加密，但是检测循序为从上到下，所以加密的时候需要从下到上的顺序进行
const unsigned char SLICE_DES = ((unsigned char)0x01 << 2); // DES加密
const unsigned char SLICE_RC5 = ((unsigned char)0x01 << 3); // RC5密码
// 其他
const unsigned char SLICE_RWD_4 = ((unsigned char)0x01 << 4);
const unsigned char SLICE_RWD_5 = ((unsigned char)0x01 << 5);
// Slice分为两种，一种是Simple，一种是Slice，客户端一般使用SimpleSilce，而服务器内部使用Slice
const unsigned char SLICE_SIMPLE = ((unsigned char)0x01 << 6);
// 是否路由过起始点，路由可以设置起始点，还未到达起始点之前都需要本起处理
const unsigned char SLICE_ROUTED_START_POINT = ((unsigned char)0x01 << 7);

#define SLICE_FLAG_GET(T) (Flags & SLICE_##T)
#define SLICE_FLAG_SET(T, v)     \
    {                            \
        if ((v))                 \
            Flags |= SLICE_##T;  \
        else                     \
            Flags &= ~SLICE_##T; \
    }

#pragma pack(push)
#pragma pack(1)

struct SimpleSlice
{
    SimpleSlice() : LengthAndFLags(0), MsgID(0) { SetSimpleSlice(true); }
    ~SimpleSlice() {}

    static SimpleSlice* CreateSlice(int nsize, _OUT int& total);

    inline int SliceLength() const { return Length; }
    inline int HeadLength() const { return sizeof(SimpleSlice); }
    inline void* Body() { return &End[0]; }
    inline int BodyLength() const { return Length >= sizeof(SimpleSlice) ? Length - sizeof(SimpleSlice) : 0; }

    inline void SetFwd(bool v) { SLICE_FLAG_SET(FWD, v); }
    inline bool IsFwd() const { return SLICE_FLAG_GET(FWD); }
    inline void SetZip(bool v) { SLICE_FLAG_SET(ZIP, v); }
    inline bool IsZipped() const { return SLICE_FLAG_GET(ZIP); }
    inline void SetDES(bool v) { SLICE_FLAG_SET(DES, v); }
    inline bool IsDES() const { return SLICE_FLAG_GET(DES); }
    inline void SetRC5(bool v) { SLICE_FLAG_SET(RC5, v); }
    inline bool IsRC5() const { return SLICE_FLAG_GET(RC5); }
    inline void SetRoutedStartPoint(bool v) { SLICE_FLAG_SET(ROUTED_START_POINT, v); }
    inline bool IsRoutedStartPoint() const { return SLICE_FLAG_GET(ROUTED_START_POINT); }
    inline void SetSimpleSlice(bool v) { SLICE_FLAG_SET(SIMPLE, v); }
    inline bool IsSimpleSlice() const { return SLICE_FLAG_GET(SIMPLE); }

    union {
        struct
        {
#ifdef LITTLE_ENDIAN
            // 这个长度包括SimpleSlice的大小，也就是说真实数据的大小为包体长-sizeof(SimpleSlice)
            int Length : 24; // 低
            int Flags : 8;   // 高
#else
            int Flags : 8;   // 高
            int Length : 24; // 低
#endif
        };
        int LengthAndFLags;
    };
    // 以下几个描述字段可以在不解包的情况下进行一些操作

    // 消息ID，不解包情况下转发需要
    // 0|-1 无效
    MsgID_t MsgID;

    char End[0];
};
#pragma pack(pop)

} // namespace XSpace

#endif // _SIMPLESLICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
