
#include "UVIdle.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnIdle(uv_idle_t* handle)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;
    
    UVIdle* self = uvdata->GetPtr<UVIdle>();
    if (NULL == self)
        return;
    
    self->OnIdle();
}

UVIdle::UVIdle(std::weak_ptr<UVLoop>& loop) : UVHandle(loop)
{
    _handle = (uv_handle_t*)Allocator::malloc(sizeof(uv_idle_t));
    if (!_loop.expired() && _handle != NULL)
    {
        uv_idle_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_idle_t*)_handle);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVIdle::~UVIdle()
{
    DEBUG("Object @%p\n", this);
}

bool UVIdle::Start()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_idle_start((uv_idle_t*)_handle, __OnIdle);
}

bool UVIdle::Stop()
{
    if (_loop.expired() || NULL == _handle)
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

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
