
#ifndef _UVCHECK_H_
#define _UVCHECK_H_

#include "UVHandle.h"

namespace XSpace
{

class UVLoop;

// Check handles will run the given callback once per loop iteration, right after polling for i/o.
class UVCheck : public UVHandle
{
public:
    UV_CREATE_HANDLE(UVCheck)

public:
    virtual ~UVCheck();

    bool Start();
    bool Stop();

    void OnClosed() OVERRIDE;
    virtual void OnCheck();

protected:
    UVCheck(const std::weak_ptr<UVLoop> &loop);
};

} // namespace XSpace

#endif // _UVCHECK_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
