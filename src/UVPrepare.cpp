
#include "UVPrepare.h"
#include "UVLoop.h"

namespace XSpace
{

static void __OnPrepare(uv_prepare_t* handle)
{
    UVData* uvdata = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == uvdata)
        return;
    
    UVPrepare *self = (UVPrepare *)uvdata->_self;
    if (NULL == self)
        return;

    self->OnPrepare();
    self->Release();
}

UVPrepare::UVPrepare(UVLoop* loop) : UVHandle(loop)
{
    _handle = (uv_handle_t*)Allocator::malloc(sizeof(uv_prepare_t));
    if (_loop != NULL && _handle != NULL)
    {
        uv_prepare_init(loop->GetRawLoop<uv_loop_t>(), (uv_prepare_t*)_handle);
        uv_handle_set_data(_handle, NULL);
        SetData(NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVPrepare::~UVPrepare()
{
    DEBUG("Object @%p\n", this);
}

void UVPrepare::Release()
{
    if (NULL == _handle)
        return;

    ClearData();
    Allocator::free(_handle);
    if (GetGC())
        delete this; // TODO:

    _handle = NULL;
}

bool UVPrepare::Start()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_prepare_start((uv_prepare_t*)_handle, __OnPrepare);
}

bool UVPrepare::Stop()
{
    if (NULL == _loop || NULL == _handle)
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

