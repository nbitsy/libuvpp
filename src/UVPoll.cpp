
#include "UVPoll.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnPoll(uv_poll_t *handle, int status, int events)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)handle);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;

    UVPoll *self = uvdata->GetPtr<UVPoll>();
    if (NULL == self)
        return;

    self->OnPoll(status, events);
}

UVPoll::UVPoll(std::weak_ptr<UVLoop> &loop, int fd) : UVHandle(loop)
{
    _handle = (uv_handle_t *)Allocator::malloc(sizeof(uv_poll_t)); // XXX: 如果很频繁请改用对象缓存池
    if (!_loop.expired() && _handle != NULL)
    {
        uv_poll_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_poll_t *)_handle, fd);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVPoll::~UVPoll()
{
    DEBUG("Object @%p\n", this);
}

bool UVPoll::Start(int events)
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_poll_start((uv_poll_t *)_handle, events, __OnPoll);
}

bool UVPoll::Stop()
{
    if (_loop.expired() || NULL == _handle)
        return false;

    return !uv_poll_stop((uv_poll_t *)_handle);
}

void UVPoll::OnClosed()
{
    DEBUG("Object @%p\n", this);
}

void UVPoll::OnPoll(int status, int events)
{
    DEBUG("Object @%p\n", this);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
