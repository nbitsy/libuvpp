
#include "UVIdle.h"
#include "UVLoop.h"

namespace XNode
{

static void __OnIdle(uv_idle_t* handle)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata)
        return;
    
    UVIdle* self = (UVIdle*)uvdata->_self;
    if (NULL == self)
        return;
    
    self->OnIdle();
    self->Release();
}

UVIdle::UVIdle(UVLoop* loop) : UVHandle(loop)
{
    _handle = (uv_handle_t*)Allocator::malloc(sizeof(uv_idle_t));
    if (_loop != NULL && _handle != NULL)
    {
        uv_idle_init(loop->GetRawLoop<uv_loop_t>(), (uv_idle_t*)_handle);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVIdle::~UVIdle()
{
    DEBUG("Object @%p\n", this);
}

void UVIdle::Release()
{
    if (NULL == _handle)
        return ;

    ClearData();
    Allocator::free(_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
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
    DEBUG("\n");
}

void UVIdle::OnIdle()
{
    DEBUG("\n");
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
