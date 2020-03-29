
#include "Allocator.h"
#include "UVAsync.h"
#include "Debugger.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnAsync(uv_async_t *async)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)async);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;
    
    UVAsync* self = uvdata->GetPtr<UVAsync>();
    if (self != NULL)
        self->OnAsync();
}

UVAsync::UVAsync(std::weak_ptr<UVLoop>& loop) : UVHandle(loop)
{
    if (_loop.expired())
        return;

    _handle = (uv_handle_t *) Allocator::Construct<uv_async_t>();
    if (_handle != NULL)
    {
        uv_async_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_async_t *)_handle, __OnAsync);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVAsync::~UVAsync()
{
    DEBUG("Object @%p\n", this);
}

bool UVAsync::Send(void* data)
{
    if (_loop.expired() || _loop.lock()->GetRawLoop<uv_loop_t>() == NULL || NULL == _handle)
        return false;
    
    Append(data);

    if (!uv_is_closing(_handle))
        return !uv_async_send(GetHandle<uv_async_t>());

    return false;
}

void UVAsync::OnClosed()
{
    DEBUG("")
}

void UVAsync::OnAsync()
{
    DEBUG("")
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
