
#include "UVReqConnect.h"
#include "Allocator.h"

namespace XSpace
{

void __OnConnected(uv_connect_t *req, int status)
{
    DEBUG("\n");
    if (NULL == req)
        return;

    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata)
        return;

    UVReqConnect *uvreqconnect = uvdata->GetPtr<UVReqConnect>();
    if (NULL == uvdata->_self)
        return;

    uvreqconnect->OnReq(status);

    // 如果Req里注册的是强引用，则释放引用，一般来说Req使用完后需要回收
    auto strong = uvdata->GetStrongPtr<UVReqConnect>();
    if (strong != NULL)
        strong->reset();
}

UVReqConnect::UVReqConnect(std::weak_ptr<UVHandle> &handle, NetAddress &address)
    : _handle(handle), _address(address)
{
    _req = (uv_req_t *)Allocator::malloc(sizeof(uv_connect_t));
    if (_req != NULL)
    {
        uv_req_set_data(_req, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVReqConnect::UVReqConnect(std::weak_ptr<UVHandle> &handle, const std::string &ip, int port)
    : _handle(handle), _address(ip, port)
{
    _req = (uv_req_t *)Allocator::malloc(sizeof(uv_connect_t));
    if (_req != NULL)
    {
        uv_req_set_data(_req, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVReqConnect::~UVReqConnect()
{
    DEBUG("Object @%p\n", this);
}

bool UVReqConnect::Start()
{
    if (NULL == _req || _handle.expired())
        return false;

    auto handle = _handle.lock();
    UVTcp* tcp = (UVTcp*)handle.get();
    int af = tcp->GetAf();
    struct sockaddr *addr = NULL;
    union {
        struct sockaddr_in _addr;
        struct sockaddr_in6 _addr6;
    };

    if (af == AF_INET6)
    {
        uv_ip6_addr(_address.Ip.c_str(), _address.Port, &_addr6);
        addr = (struct sockaddr *)&_addr6;
    }
    else
    {
        uv_ip4_addr(_address.Ip.c_str(), _address.Port, &_addr);
        addr = (struct sockaddr *)&_addr;
    }

    if (NULL == addr)
        return false;

    return uv_tcp_connect(GetReq<uv_connect_t>(), handle->GetHandle<uv_tcp_t>(), addr, __OnConnected) == 0;
}

void UVReqConnect::OnReq(int status)
{
    DEBUG("\n");
    if (!_handle.expired())
    {
        auto handle = _handle.lock();
        UVTcp* t = (UVTcp*)handle.get();
        if (status == 0)
        {
            t->OnConnectedAction();
            t->OnConnected();
        }
        else
        {
            t->OnErrorAction(status);
            t->OnError(status);
        }
    }
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
