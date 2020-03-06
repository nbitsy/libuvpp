
#ifndef UVREQSHUTDOWN_H_
#define UVREQSHUTDOWN_H_

#include "UVReq.h"

namespace XNode
{

class UVStream;

class UVReqShutdown : public UVReq
{
public:
    UVReqShutdown(UVStream* stream);
    ~UVReqShutdown();

    bool Begin();
    void OnReq(int status);

private:
    UVStream* _stream;
};

} // namespace XNode

#endif // UVREQSHUTDOWN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

