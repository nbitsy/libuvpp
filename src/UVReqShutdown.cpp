
#include "UVReqShutdown.h"
#include "UVStream.h"

namespace XSpace
{

void __OnShutdown(uv_shutdown_t *req, int status)
{
    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata)
        return;
    
    UVReqShutdown* uvreqshutdown = (UVReqShutdown*)uvdata->_self;
    if (NULL == uvreqshutdown)
        return;

    uvreqshutdown->OnReq(status);
    uvreqshutdown->Release();
}

UVReqShutdown::UVReqShutdown(UVStream *stream)
{
    _req = (uv_req_t*)Allocator::malloc(sizeof(uv_shutdown_t));
    if (_req != NULL)
    {
        uv_req_set_data(_req, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVReqShutdown::~UVReqShutdown()
{
}

bool UVReqShutdown::Start()
{
    if (NULL == _req || NULL == _stream)
        return false;

    return uv_shutdown(GetReq<uv_shutdown_t>(), _stream->GetHandle<uv_stream_t>(), __OnShutdown) == 0;
}

void UVReqShutdown::OnReq(int status)
{
    DEBUG("\n");
}

void UVReqShutdown::Release()
{
    if (NULL == _req)
        return;

    ClearData();
    Allocator::free(_req);
    if (GetGC())
        delete this; // TODO:
    
    _req = NULL;
}

UVLoop *UVReqShutdown::GetLoop()
{
    if (NULL == _stream)
        return NULL;

    return _stream->GetLoop();
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
