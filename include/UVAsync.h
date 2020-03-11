
#ifndef _UVASYNC_H_
#define _UVASYNC_H_

#include "UVHandle.h"

namespace XNode
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

    bool Send();

    virtual void OnAsync();
    void OnClosed();
    void Release();

private:
    void AppendData(void *data);
};

} // namespace XNode

#endif // _UVASYNC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
