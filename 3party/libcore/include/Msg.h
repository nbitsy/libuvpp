
#ifndef _MES_H_
#define _MES_H_

namespace XSpace
{

class Msg
{
public:
    Msg() {}
    Msg(unsigned short msgid) : _msgid(msgid) {}
    ~Msg() {}

    inline unsigned short MsgID() const { return _msgid; }
    inline void SetMsgID(unsigned short msgid) { _msgid = msgid; }

    inline unsigned int MsgLength() const { return _msglen; }
    inline void SetMsgLength(unsigned int len) { _msglen = len; }

    inline unsigned short& FLags() { return _flags; }
    inline void SetFlags(unsigned short flag) { _flags = flag; }


protected:
    unsigned short _msgid = 0;  // 消息编号
    unsigned short _flags = 0;   // 标记位具体应用自己解释它的作用
    unsigned int _msglen = 0;   // 不包括Msg大小的消息长
};

} // namespace XSpace

#endif // _MES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

