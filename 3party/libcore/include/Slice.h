
#ifndef _SLICE_H_
#define _SLICE_H_

#include "Allocator.h"
#include "Config.h"
#include "CoreConfig.h"
#include "SimpleSlice.h"

namespace XSpace
{

enum ESliceType
{
    EST_NONE = 0,
    EST_SLICE,
    EST_SIMPLE_SLICE,
};

/**
 * Slice是最低层的通信包结构，很简单，只有一个长度，所有数据跟在长度后面，而Msg是基于
 * Slice的实现，对于一个Slice的数据包来说，他的包内容可以是Msg，或者其他种类的Msg。
 * 基于TCP协议的连接收到的一个Slice可能是不完整的，所以需要进行拼包处理。但是基于UDP
 * 协议的连接一个Slice就是一个包，因为UDP不分包。
*/

/**
 * 对于一个Slice对象来说，本质上是一个变长对象，但分配对象内存时，需要把分片的长度加上去
*/

#pragma pack(push)
#pragma pack(1)
struct Slice : public SimpleSlice
{
    Slice() : SimpleSlice(), FwdType(0), FwdTargetType(0), FwdTarget(0), Target(0) { SetSimpleSlice(false); }
    ~Slice() {}

    static Slice* CreateSlice(int nsize, _OUT int& total);
    inline int HeadLength() const { return sizeof(Slice); }
    inline void* Body() { return &End[0]; }
    inline unsigned int BodyLength() const { return Length >= sizeof(Slice) ? Length - sizeof(Slice) : 0; }
    inline bool NeedForward() const { return FwdTargetType > 0; }

    // 转发类型，经过的路径一般是多条的，如何选择一条路径由FwdType来决定
    // 0 - first
    // 1 - hash 根据Target进行hash
    // 2 - random
    // 3 - target
    // other hash
    unsigned char FwdType;
    // 转发目标的节点类型，如果当前节点类型为FwdTargetType，则启用FwdTarget条件
    // -1 广播(0xff)
    // 0 不转发
    // other 转发去的目标服务器类型
    unsigned char FwdTargetType;
    // 如果类型匹配后，当前节点标识与FwdTarget相等时启用Target条件
    // ull主要是因为服务器的节点ID是ull的
    // -1 广播(0xffffffffffffffff)
    unsigned long long FwdTarget;
    // 目标ID，如玩家ID
    // ull为目标ID预留空间，一般是unsigned int的
    // -1 广播(0xffffffffffffffff)
    unsigned long long Target;
    // 之后为包体
    char End[0];
};
#pragma pack(pop)

} // namespace XSpace

#endif // _SLICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
