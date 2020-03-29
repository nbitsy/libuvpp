
#include "UVStream.h"
#include "Allocator.h"
#include "UVReqShutdown.h"

namespace XSpace
{

void __OnShutdown(uv_shutdown_t *req, int status)
{
    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata)
        return;
    
    UVReqShutdown* uvreqshutdown = uvdata->GetPtr<UVReqShutdown>();
    if (NULL == uvreqshutdown)
        return;

    uvreqshutdown->OnReq(status);
}

UVReqShutdown::UVReqShutdown(std::weak_ptr<UVHandle>& handle)
{
    _req = (uv_req_t*)Allocator::malloc(sizeof(uv_shutdown_t));
    if (_req != NULL)
    {
        uv_req_set_data(_req, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVReqShutdown::~UVReqShutdown()
{
}

bool UVReqShutdown::Start()
{
    if (NULL == _req || _handle.expired())
        return false;

    return uv_shutdown(GetReq<uv_shutdown_t>(), _handle.lock()->GetHandle<uv_stream_t>(), __OnShutdown) == 0;
}

void UVReqShutdown::OnReq(int status)
{
    DEBUG("\n");
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
