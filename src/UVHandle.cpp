
#include "UVHandle.h"
#include <iostream>

namespace XSpace
{

static void __OnClosed(uv_handle_t* handle)
{
    DEBUG("Free @%p\n", handle);
    UVData* uvdata = (UVData*)handle->data;
    if (NULL == uvdata)
        WARN(" %p 's owner has released already.", handle);

    UVHandle* h = uvdata->GetPtr<UVHandle>();
    if (NULL == h)
        return;

    h->OnClosed();

    Allocator::free(handle);
}

UVHandle::UVHandle(const std::weak_ptr<UVLoop>& loop) : _loop(loop), _handle(NULL)
{
    DEBUG("Object @%p\n", this);
}

UVHandle::~UVHandle()
{
    /**
     * 如果Handle对象已经被释放，但是事件后面才被调起则会出现意外情况
     * 所以，Handle对象的回收都是通过延迟回收来实现，就是在延迟回收的
     * 流程里，如果Handle的状态为closed时，则说明可以回收。
    */
    _handle = NULL;
    DEBUG("Object @%p\n", this);
}

void UVHandle::SetData(void* data, bool force, bool strong)
{
    UVDataHelper::SetData(this, _handle, data, force, strong);
}

const UVData* UVHandle::GetData() const
{
    return UVDataHelper::GetData(_handle);
}

void UVHandle::ClearData()
{
    UVDataHelper::ClearData(_handle);
}

bool UVHandle::IsActive() const
{
    if (NULL == _handle)
        return false;

    return uv_is_active(_handle);
}

bool UVHandle::IsClosing() const
{
    if (NULL == _handle)
        return true;

    return uv_is_closing(GetHandle<uv_handle_t>());
}

int UVHandle::RawFd() const
{
    int fd = 0;
    if (uv_fileno(GetHandle<uv_handle_t>(), &fd) == 0)
        return fd;

    return -1;
}

void UVHandle::Close()
{
    if (_handle != NULL && !IsClosing())
        uv_close(_handle, __OnClosed);
}

int UVHandle::SendBufferSize() const
{
    if (NULL == _handle)
        return 0;

    int value = 0;
    uv_send_buffer_size(_handle, &value);
    return value;
}

int UVHandle::RecvBufferSize() const
{
    if (NULL == _handle)
        return 0;

    int value = 0;
    uv_recv_buffer_size(_handle, &value);
    return value;
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
