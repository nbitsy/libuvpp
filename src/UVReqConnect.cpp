
#include "UVReqConnect.h"

namespace XNode
{

void __OnConnected(uv_connect_t *req, int status)
{
    if (NULL == req)
        return;

    UVData* uvdata = (UVData*)uv_req_get_data((uv_req_t*)req);
    if (NULL == uvdata)
        return;
    
    if (uvdata->_self != NULL)
        ((UVReqConnect*)uvdata->_self)->OnReq(status);
}

UVReqConnect::UVReqConnect(UVTcp *handle, EndPointAddress address) : _handle(handle), _address(address)
{
}

UVReqConnect::UVReqConnect(UVTcp *handle, const std::string &ip, int port) : _handle(handle), _address(ip, port)
{
}

UVReqConnect::~UVReqConnect()
{
}

bool UVReqConnect::Start()
{
    if (NULL == _req || NULL == _handle)
        return false;

    int af = _handle->GetAf();
    struct sockaddr *addr = NULL;
    union {
        struct sockaddr_in _addr;
        struct sockaddr_in6 _addr6;
    };

    if (af == AF_INET6)
    {
        uv_ip6_addr(_address._ip.c_str(), _address._port, &_addr6);
        addr = (struct sockaddr *)&_addr6;
    }
    else
    {
        uv_ip4_addr(_address._ip.c_str(), _address._port, &_addr);
        addr = (struct sockaddr *)&_addr;
    }

    if (NULL == addr)
        return false;

    return uv_tcp_connect(GetReq<uv_connect_t>(), _handle->GetHandle<uv_tcp_t>(), addr, __OnConnected) == 0;
}

void UVReqConnect::OnReq(int status)
{
    if (_handle != NULL)
        _handle->OnConnected();

    if (GetGC())
        delete this; // TODO
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
