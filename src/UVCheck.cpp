
#include "UVCheck.h"
#include "UVLoop.h"

namespace XNode
{

static void __OnCheck(uv_check_t *handle)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)handle);
    if (NULL == uvdata)
        return;
    
    UVCheck* self = (UVCheck*)uvdata->_self;
    if (NULL == self)
        return;
    
    self->OnCheck();
    self->Release();
}

UVCheck::UVCheck(UVLoop *loop) : UVHandle(loop)
{
    _handle = (uv_handle_t *)Allocator::malloc(sizeof(uv_check_t)); // XXX: 频率很低所有直接使用Allocator::malloc
    if (_loop != NULL && _handle != NULL)
    {
        uv_check_init(loop->GetRawLoop<uv_loop_t>(), (uv_check_t *)_handle);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p => ", (void *)this);
}

UVCheck::~UVCheck()
{
    DEBUG("Object @%p => ", (void *)this);
}

void UVCheck::Release()
{
    if (NULL == _handle)
        return;

    ClearData();
    Allocator::free(_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

bool UVCheck::Start()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_check_start((uv_check_t *)_handle, __OnCheck);
}

bool UVCheck::Stop()
{
    if (NULL == _loop || NULL == _handle)
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

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
