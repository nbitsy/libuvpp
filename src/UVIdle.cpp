
#include "UVIdle.h"

namespace XNode
{

static void __OnIdle(uv_idle_t* handle)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata)
        return;
    
    if (uvdata->_self != NULL)
        ((UVIdle*)uvdata->_self)->OnIdle();
}

UVIdle::UVIdle(UVLoop* loop) : UVHandle(loop)
{
    _handle = (uv_handle_t*)malloc(sizeof(uv_idle_t));
    if (_loop != NULL && _handle != NULL)
        uv_idle_init(loop->GetLoop<uv_loop_t>(), (uv_idle_t*)_handle);
    
    SetData(NULL);
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVIdle::~UVIdle()
{
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

bool UVIdle::Start()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_idle_start((uv_idle_t*)_handle, __OnIdle);
}

bool UVIdle::Stop()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_idle_stop((uv_idle_t*)_handle);
}

void UVIdle::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void UVIdle::OnIdle()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
