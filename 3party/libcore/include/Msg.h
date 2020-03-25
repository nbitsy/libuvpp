
#ifndef _MES_H_
#define _MES_H_

/**
 * Msg是对于Slice来说相对上层一点，一个Msg肯定是一个完整数据包，不用考虑粘包问题。
 * Msg内部的内容可以是一个protobuf格式的数据，或者其他自定义格式。
*/

namespace XSpace
{

const unsigned short MSG_FLAG_DYN = ((unsigned short)0x1 << 0); // 是否是变长包

#define MSG_FLAG_GET(T) (MsgFlags & MSG_FLAG_##T)
#define MSG_FLAG_SET(T, v)             \
    {                                  \
        if ((v))                       \
            MsgFlags |= MSG_FLAG_##T;  \
        else                           \
            MsgFlags &= ~MSG_FLAG_##T; \
    }

struct Msg
{
    Msg() {}
    Msg(unsigned short msgid) : MsgID(msgid) {}
    ~Msg() {}

    inline bool IsDynamic() const { return MSG_FLAG_GET(DYN); }
    inline void SetDynamic(bool v) { MSG_FLAG_SET(DYN, v); }

    // 对于变长表来说，消息休就跟随在包头后面，固定长数据包不需要这个功能
    inline void *Body() { return &MsgFlags + 1; }
    inline unsigned short BodyLength() const { return MsgLength >= sizeof(Msg) ? MsgLength - sizeof(Msg) : 0; }

    unsigned short MsgLength; // 消息长
    unsigned short MsgID;     // 消息编号
    unsigned int MsgSeq;      // 消息顺号
    unsigned short MsgFlags;  // 标记位具体应用自己解释它的作用
};

} // namespace XSpace

#endif // _MES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
