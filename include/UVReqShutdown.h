
#ifndef _UVREQSHUTDOWN_H_
#define _UVREQSHUTDOWN_H_

#include "UVReq.h"

namespace XSpace
{

class UVStream;

class UVReqShutdown : public UVReq
{
public:
    UVReqShutdown(UVStream* stream);
    ~UVReqShutdown();

    UVLoop *GetLoop();
    bool Start();
    void OnReq(int status);
    void Release();

private:
    UVStream* _stream;
};

} // namespace XSpace

#endif // _UVREQSHUTDOWN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
