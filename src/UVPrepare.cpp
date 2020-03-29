
#include "UVPrepare.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnPrepare(uv_prepare_t* handle)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;
    
    UVPrepare* self = uvdata->GetPtr<UVPrepare>();
    if (NULL == self)
        return;
    
    self->OnPrepare();
}

UVPrepare::UVPrepare(std::weak_ptr<UVLoop>& loop) : UVHandle(loop)
{
    _handle = (uv_handle_t*)Allocator::malloc(sizeof(uv_prepare_t));
    if (!_loop.expired() && _handle != NULL)
    {
        uv_prepare_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_prepare_t*)_handle);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVPrepare::~UVPrepare()
{
    DEBUG("Object @%p\n", this);
}

bool UVPrepare::Start()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_prepare_start((uv_prepare_t*)_handle, __OnPrepare);
}

bool UVPrepare::Stop()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_prepare_stop((uv_prepare_t*)_handle);
}

void UVPrepare::OnClosed()
{
    DEBUG("Object @%p\n", this);
}

void UVPrepare::OnPrepare()
{
    DEBUG("Object @%p\n", this);
}

} // namespace XSpace


/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

