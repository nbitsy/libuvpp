
#include "UVReqShutdown.h"
#include "UVStream.h"

namespace XNode
{

void __OnShutdown(uv_shutdown_t *req, int status)
{
    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (uvdata != NULL && uvdata->_self != NULL)
        ((UVReqShutdown *)uvdata->_self)->OnReq(status);
}

UVReqShutdown::UVReqShutdown(UVStream *stream)
{
}

UVReqShutdown::~UVReqShutdown()
{
}

bool UVReqShutdown::Begin()
{
    if (NULL == _req || NULL == _stream)
        return false;

    return uv_shutdown(GetReq<uv_shutdown_t>(), _stream->GetHandle<uv_stream_t>(), __OnShutdown) == 0;
}

void UVReqShutdown::OnReq(int status)
{
    if (GetGC())
        delete this; // TODO: delete or pool release
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
