
#ifndef _UVIDLE_H_
#define _UVIDLE_H_

#include "UVHandle.h"

namespace XSpace
{

class UVLoop;

// called after per loop
class UVIdle : public UVHandle
{
public:
    UV_CREATE_HANDLE(UVIdle)

public:
    virtual ~UVIdle();

    bool Start();
    bool Stop();

    void OnClosed() OVERRIDE;
    virtual void OnIdle();

protected:
    UVIdle(const std::weak_ptr<UVLoop> &loop);
};

} // namespace XSpace

#endif // _UVIDLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
