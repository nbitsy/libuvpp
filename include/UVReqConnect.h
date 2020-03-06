
#ifndef UVREQCONNECT_H_
#define UVREQCONNECT_H_

#include "UVReq.h"
#include "UVTypes.h"
#include "UVTcp.h"

namespace XNode
{

class UVReqConnect : public UVReq
{
public:
    UVReqConnect(UVTcp* handle, EndPointAddress address);
    UVReqConnect(UVTcp* handle, const std::string& ip, int port);
    ~UVReqConnect();

    bool Begin();
    void OnReq(int status);

private:
    UVTcp* _handle;
    EndPointAddress _address;
};

} // namespace XNode

#endif // UVREQCONNECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

