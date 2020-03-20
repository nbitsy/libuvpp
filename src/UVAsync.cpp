
#include "UVAsync.h"
#include "Debuger.h"
#include "UVLoop.h"

namespace XSpace
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
    self->Release();
}

UVAsync::UVAsync(UVLoop *loop) : UVHandle(loop)
{
    if (NULL == _loop)
        return;

    _handle = (uv_handle_t *)loop->Construct<uv_async_t>();
    if (_loop != NULL && _handle != NULL)
    {
        uv_async_init(loop->GetRawLoop<uv_loop_t>(), (uv_async_t *)_handle, __OnAsync);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVAsync::~UVAsync()
{
    DEBUG("Object @%p\n", this);
}

void UVAsync::Release()
{
    auto loop = GetLoop();
    if (NULL == loop)
    {
        return;
    }

    ClearData();
    loop->Destroy((uv_async_t*)_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

bool UVAsync::Send(void* data)
{
    if (NULL == _loop || _loop->GetRawLoop<uv_loop_t>() == NULL || NULL == _handle)
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
