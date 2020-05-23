
#ifndef _SLICE_H_
#define _SLICE_H_

#include "Config.h"

namespace XSpace
{

/**
 * Slice是最低层的通信包结构，很简单，只有一个长度，所有数据跟在长度后面，而Msg是基于
 * Slice的实现，对于一个Slice的数据包来说，他的包内容可以是Msg，或者其他种类的Msg。
 * 基于TCP协议的连接收到的一个Slice可能是不完整的，所以需要进行拼包处理。但是基于UDP
 * 协议的连接一个Slice就是一个包，因为UDP不分包。
*/

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

#pragma pack(push)
#pragma pack(4)
struct Slice
{
    Slice() : MsgID(0), FwdType(0), FwdTargetType(0), FwdTarget(0), Target(0) {}
    ~Slice() {}

    inline int SliceLength() const { return Length; }
    inline int HeadLength() const { return sizeof(Slice); }
    inline void* Body() { return &End[0]; }
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
            int Flags : 8;   // 高
            int Length : 24; // 低
#endif
        };
    };
    // 以下几个描述字段可以在不解包的情况下进行一些操作

    inline bool NeedForward() const { return FwdTargetType > 0; }

    // 消息ID，客户有这个才知道怎么解释
    // 0|-1 无效
    unsigned int MsgID;

    struct
    {
        // 转发类型，经过的路径一般是多条的，如果选择一条路径由FwdType来决定
        // 0 - first
        // 1 - hash 根据Target进行hash
        // 2 - random
        // other hash
        unsigned short FwdType;
        // 转发目标的节点类型，如果当前节点类型为FwdTargetType，则启用FwdTarget条件
        // -1 广播
        // 0 不转发
        // other 转发去的目标服务器类型
        unsigned short FwdTargetType;
    };

    // 如果类型匹配后，当前节点标识与FwdTarget相等时启用Target条件
    // -1 广播
    unsigned long long FwdTarget;
    // 目标ID，如玩家ID
    // -1 广播
    unsigned long long Target;

    // 之后为包体
    char End[0];
};
#pragma pack(pop)

} // namespace XSpace

#endif // _SLICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
