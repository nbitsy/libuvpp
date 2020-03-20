
#include "UVHandle.h"
#include <iostream>

namespace XSpace
{

static void __OnClosed(uv_handle_t *handle)
{
    UVData* data = (UVData*)handle->data;
    if (NULL == data)
        std::cout << "@" << handle << " 's owner has released already." << std::endl;

    UVHandle* uvhandle = (UVHandle*)data->_self;
    if (data != NULL && uvhandle != NULL)
        uvhandle->OnClosed();

    DEBUG("Free @%p\n", handle);

    // 如果没用调用到Release可能会导致资源泄漏
    if (uvhandle != NULL)
        uvhandle->Release();
}

UVHandle::UVHandle(UVLoop* loop) : _loop(loop), _handle(NULL)
{
    DEBUG("Object @%p\n", this);
}

UVHandle::~UVHandle()
{
    /**
     * 如果Handle对象已经被释放，但是事件后面才被调起则非去现意外情况
     * 那么在对像被释放的时候就发起关闭事件，并且清空数据对象，后面事件
     * 的调起也不会有任何问题
    */
    Close();
    // XXX: handle本身的回收需要到确定的handle的析构函数里去处理
    DEBUG("Object @%p\n", this);
}

void UVHandle::SetData(void *data, bool force)
{
    UVDataHelper::SetData(_handle, data, force);
}

const UVData *UVHandle::GetData() const
{
    return UVDataHelper::GetData(_handle);
}

void UVHandle::ClearData()
{
    UVDataHelper::ClearData(_handle);
}

#if 0
UVLoop* UVHandle::GetLoop()
{
    if (NULL == _handle)
        return NULL;
    
    uv_loop_t* loop = (uv_loop_t*)uv_handle_get_loop(_handle);
    if (NULL == loop)
        return NULL;
    
    UVData* pdata = (UVData*)uv_loop_get_data(loop);
    if (NULL == pdata)
        return NULL;
    
    return (UVLoop*)pdata->_self;
}
#endif

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

void UVHandle::Release()
{
    DEBUG("Release");
    delete this; // TODO:
}

} // namespace XSpace


/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

