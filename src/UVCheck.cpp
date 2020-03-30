
#include "UVCheck.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnCheck(uv_check_t *handle)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)handle);
    if (NULL == uvdata)
        return;

    auto self = uvdata->GetPtr<UVCheck>();
    if (self != NULL)
        self->OnCheck();
}

UVCheck::UVCheck(const std::weak_ptr<UVLoop> &loop) : UVHandle(loop)
{
    _handle = (uv_handle_t *)Allocator::malloc(sizeof(uv_check_t)); // XXX: 频率很低所有直接使用Allocator::malloc
    if (!_loop.expired() && _handle != NULL)
    {
        uv_check_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_check_t *)_handle);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p => ", (void *)this);
}

UVCheck::~UVCheck()
{
    DEBUG("Object @%p => ", (void *)this);
}

bool UVCheck::Start()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_check_start((uv_check_t *)_handle, __OnCheck);
}

bool UVCheck::Stop()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_check_stop((uv_check_t *)_handle);
}

void UVCheck::OnClosed()
{
    DEBUG("\n");
}

void UVCheck::OnCheck()
{
    DEBUG("\n");
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
