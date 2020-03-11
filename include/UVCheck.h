
#ifndef _UVCHECK_H_
#define _UVCHECK_H_

#include "UVHandle.h"

namespace XNode
{

class UVLoop;

// Check handles will run the given callback once per loop iteration, right after polling for i/o.
class UVCheck : public UVHandle
{
public:
    UVCheck(UVLoop* loop);
    virtual ~UVCheck();

    bool Start();
    bool Stop();

    void OnClosed();
    virtual void OnCheck();
    void Release();
};

} // namespace XNode

#endif // _UVCHECK_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
