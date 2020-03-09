
#include "UVAsync.h"

namespace XNode
{

static void __OnAsync(uv_async_t *async)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)async);
    if (NULL == uvdata)
        return;
    
    UVAsync* self = (UVAsync*)uvdata->_self;
    if (NULL == self)
        return;
    
    self->OnAsync();
}

UVAsync::UVAsync(UVLoop *loop) : UVHandle(loop)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_async_t));
    if (_loop != NULL && _handle != NULL)
        uv_async_init(loop->GetLoop<uv_loop_t>(), (uv_async_t *)_handle, __OnAsync);

    SetData(NULL);
    std::cout << "Object@" << (void *)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVAsync::~UVAsync()
{
    std::cout << "Object@" << (void *)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

bool UVAsync::Send()
{
    if (NULL == _loop || _loop->GetLoop<uv_loop_t>() == NULL || NULL == _handle)
        return false;

    if (!uv_is_closing(_handle))
        return !uv_async_send(GetHandle<uv_async_t>());

    return false;
}

void UVAsync::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void UVAsync::OnAsync()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
