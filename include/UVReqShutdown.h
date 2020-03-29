
#ifndef _UVREQSHUTDOWN_H_
#define _UVREQSHUTDOWN_H_

#include "UVReq.h"

namespace XSpace
{

class UVStream;

class UVReqShutdown : public UVReq
{
public:
    UV_CREATE_REQ_STRONG(UVReqShutdown)

public:
    UVReqShutdown(std::weak_ptr<UVHandle> &handle);
    ~UVReqShutdown();

    bool Start() OVERRIDE;
    void OnReq(int status) OVERRIDE;

private:
    std::weak_ptr<UVHandle> _handle;
};

} // namespace XSpace

#endif // _UVREQSHUTDOWN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
