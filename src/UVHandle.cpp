
#include "UVHandle.h"

namespace XNode
{

static void __OnClose(uv_handle_t *handle)
{
    UVData* data = (UVData*)handle->data;
    if (data->_self)
        (((UVHandle*)data->_self))->OnClose();
}

UVHandle::UVHandle() : _handle(NULL)
{
    SetData(NULL, true); // set this
}

UVHandle::~UVHandle()
{
    const UVData* pdata = (UVData*)GetData();
    if (pdata != NULL)
        delete pdata; // TODO:
    // XXX: handle本身的回收需要到确定的handle的析构函数里去处理
}

void UVHandle::SetData(void *data, bool force)
{
    UVDataHelper::SetData(_handle, data, force);
}

const UVData *UVHandle::GetData() const
{
    return UVDataHelper::GetData(_handle);
}

UVLoop* UVHandle::GetLoop() const
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
    if (_handle != NULL)
        uv_close(_handle, __OnClose);
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

} // namespace XNode


/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

