
#ifndef UVIDLE_H_
#define UVIDLE_H_

#include "UVHandle.h"

namespace XNode
{

class UVLoop;

// called after per loop
class UVIdle : public UVHandle
{
public:
    UVIdle(UVLoop* loop);
    virtual ~UVIdle();

    bool Start();
    bool Stop();

    virtual void OnClosed();
    virtual void OnIdle();
};

} // namespace XNode

#endif // UVIDLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

