
#ifndef _UVREQCONNECT_H_
#define _UVREQCONNECT_H_

#include "UVReq.h"
#include "NetAddress.h"
#include "UVTcp.h"

namespace XSpace
{

class UVReqConnect : public UVReq
{
public:
    UVReqConnect(UVTcp* handle, NetAddress address);
    UVReqConnect(UVTcp* handle, const std::string& ip, int port);
    ~UVReqConnect();

    UVLoop *GetLoop();

    bool Start();
    void OnReq(int status);
    void Release();

private:
    UVTcp* _handle;
    NetAddress _address;
};

} // namespace XSpace

#endif // _UVREQCONNECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

