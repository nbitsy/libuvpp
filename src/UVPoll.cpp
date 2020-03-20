
#include "UVPoll.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnPoll(uv_poll_t *handle, int status, int events)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)handle);
    if (NULL == uvdata)
        return;

    UVPoll *self = (UVPoll *)uvdata->_self;
    if (NULL == self)
        return;

    self->OnPoll(status, events);
    self->Release();
}

UVPoll::UVPoll(UVLoop *loop, int fd) : UVHandle(loop)
{
    _handle = (uv_handle_t *)Allocator::malloc(sizeof(uv_poll_t)); // XXX: 如果很频繁请改用对象缓存池
    if (_loop != NULL && _loop->GetRawLoop<uv_loop_t>() != NULL && _handle != NULL)
    {
        uv_poll_init(loop->GetRawLoop<uv_loop_t>(), (uv_poll_t *)_handle, fd);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVPoll::~UVPoll()
{
    DEBUG("Object @%p\n", this);
}

void UVPoll::Release()
{
    if (NULL == _handle)
        return;

    ClearData();
    Allocator::free(_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

bool UVPoll::Start(int events)
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_poll_start((uv_poll_t *)_handle, events, __OnPoll);
}

bool UVPoll::Stop()
{
    if (NULL == _loop || NULL == _handle)
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
