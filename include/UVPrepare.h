
#ifndef _UVPREPARE_H_
#define _UVPREPARE_H_

#include "UVHandle.h"

namespace XSpace
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

    void OnClosed();
    virtual void OnPrepare();
    void Release();
};

} // namespace XSpace

#endif // _UVPREPARE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

