
#include "UVPoll.h"

namespace XNode
{

static void __OnPoll(uv_poll_t* handle, int status, int events)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata)
        return;
    
    if (uvdata->_self != NULL)
        ((UVPoll*)uvdata->_self)->OnPoll(status, events);
}

UVPoll::UVPoll(UVLoop* loop, int fd) : UVHandle(loop)
{
    _handle = (uv_handle_t*)malloc(sizeof(uv_poll_t));
    if (_loop != NULL && _loop->GetLoop<uv_loop_t>() != NULL && _handle != NULL)
        uv_poll_init(loop->GetLoop<uv_loop_t>(), (uv_poll_t*)_handle, fd);
    
    SetData(NULL);
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVPoll::~UVPoll()
{
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

bool UVPoll::Start(int events)
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_poll_start((uv_poll_t*)_handle, events, __OnPoll);
}

bool UVPoll::Stop()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_poll_stop((uv_poll_t*)_handle);
}

void UVPoll::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void UVPoll::OnPoll(int status, int events)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace XNode


/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

