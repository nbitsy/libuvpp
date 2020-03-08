
#ifndef UVPREPARE_H_
#define UVPREPARE_H_

#include "UVHandle.h"

namespace XNode
{

class UVLoop;

// called after per loop
class UVPrepare : public UVHandle
{
public:
    UVPrepare(UVLoop* loop);
    virtual ~UVPrepare();

    bool Start();
    bool Stop();

    virtual void OnClosed();
    virtual void OnPrepare();
};

} // namespace XNode

#endif // UVPREPARE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

