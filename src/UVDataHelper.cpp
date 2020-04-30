
#include "UVDataHelper.h"
#include "UVLoop.h"
#include <iostream>

namespace XSpace
{

UVData::UVData(bool strong, void* self, void* data)
    : _strong(strong), _self(self), _data(data)
{
    DEBUG("Object @%p _self: %p, _data: %p\n", this, _self, _data);
}

UVData::~UVData()
{
    DEBUG("Object @%p _self: %p, _data: %p\n", this, _self, _data);
#if _DEBUG
    if (_self != NULL)
    {
        if (_strong)
        {
            auto self = ((std::shared_ptr<UVDataHelper>*)_self)->get();
            DEBUG("_self: %p, realself: %p, _data: %p\n", _self, self, _data);
        }
        else
        {
            auto self = ((std::weak_ptr<UVDataHelper>*)_self)->lock().get();
            DEBUG("_self: %p, realself: %p, _data: %p\n", _self, self, _data);
        }
    }
    else
    {
        DEBUG("_self: %p, realself: 0x00000000, _data: %p\n", _self, _data);
    }
#endif

    if (_self != NULL)
    {
        if (_strong)
            Allocator::Destroy(GetSharedPtr<UVDataHelper>());
        else
            Allocator::Destroy(GetWeakPtr<UVDataHelper>());
        _self = NULL;
    }
}

void UVDataHelper::ClearData(void* target, int type)
{
    if (NULL == target)
        return;

    UVData* uvdata = GetData(target, type);
    if (NULL == uvdata)
        return;

    Allocator::Destroy(uvdata);

    switch (type)
    {
    case UVDT_LOOP:
        uv_loop_set_data((uv_loop_t*)target, NULL);
        break;
    case UVDT_HANDLE:
        uv_handle_set_data((uv_handle_t*)target, NULL);
        break;
    case UVDT_REQ:
        uv_req_set_data((uv_req_t*)target, NULL);
        break;
    default:
        break;
    }
}

UVData* UVDataHelper::GetData(void* target, int type) const
{
    if (NULL == target)
        return NULL;

    switch (type)
    {
    case UVDT_LOOP:
        return (UVData*)uv_loop_get_data((uv_loop_t*)target);
    case UVDT_HANDLE:
        return (UVData*)uv_handle_get_data((uv_handle_t*)target);
    case UVDT_REQ:
        return (UVData*)uv_req_get_data((uv_req_t*)target);
    default:
        break;
    }
    return NULL;
}

void UVDataHelper::BufAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    UVData* data = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == buf || NULL == data)
    {
        buf->base = NULL;
        buf->len = 0;
        return;
    }

    buf->base = (char*)Allocator::malloc(suggested_size);
    buf->len = suggested_size;

    DEBUG("OnRead alloc @%p size: %lu\n", buf->base, buf->len);
}

void UVDataHelper::BufFree(const uv_buf_t* buf)
{
    if (NULL == buf || NULL == buf->base)
        return;

    DEBUG("OnRead free @%p\n", buf->base);
    Allocator::free(buf->base);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
