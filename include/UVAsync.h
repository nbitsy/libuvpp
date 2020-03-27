
#ifndef _UVASYNC_H_
#define _UVASYNC_H_

#include "UVHandle.h"

namespace XSpace
{

class UVLoop;

/**
 * 在每一次的Notify主会调起
*/

class UVAsync : public UVHandle
{
public:
    explicit UVAsync(UVLoop *loop);
    virtual ~UVAsync();

    bool Send(void* data);

    void OnClosed() OVERRIDE;
    void Release() OVERRIDE;

    // 同步消息调起后处理函数
    virtual void OnAsync();
    // 放入同步对象的数据怎么处理
    virtual void Append(void* data) = 0;
};

} // namespace XSpace

#endif // _UVASYNC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
