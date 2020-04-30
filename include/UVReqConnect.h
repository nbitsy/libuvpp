
#ifndef _UVREQCONNECT_H_
#define _UVREQCONNECT_H_

#include "NetAddress.h"
#include "UVReq.h"
#include "UVTcp.h"

namespace XSpace
{

class UVReqConnect : public UVReq
{
public:
    UV_CREATE_REQ_SHARED(UVReqConnect)

public:
    UVReqConnect(const std::weak_ptr<UVHandle>& handle, const NetAddress& address);
    UVReqConnect(const std::weak_ptr<UVHandle>& handle, const std::string& ip, int port);
    ~UVReqConnect();

    bool Start() OVERRIDE;
    void OnReq(int status) OVERRIDE;

private:
    std::weak_ptr<UVHandle> _handle;
    NetAddress _address;
};

} // namespace XSpace

#endif // _UVREQCONNECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
