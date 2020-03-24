
#include "UVDataHelper.h"
#include "UVLoop.h"
#include <iostream>

namespace XSpace
{

void UVDataHelper::SetData(void *target, void *data, bool force, int type)
{
    if (NULL == target)
        return;

    auto loop = GetLoop();
    if (NULL == loop)
    {
        std::cerr << "GetLoop get a null value!!!" << std::endl;
        return;
    }

    UVData *uvdata = GetData(target, type);
    if (uvdata != NULL && force)
    {
        loop->Destroy(uvdata);
        uvdata = loop->Construct<UVData>();
    }
    else if (NULL == uvdata)
    {
        uvdata = loop->Construct<UVData>();
    }

    if (NULL == uvdata)
    {
        std::cerr << "ERROR: alloc UVData!!!" << std::endl;
        return;
    }

    uvdata->_self = this;
    uvdata->_data = data;

    switch (type)
    {
    case UVDT_LOOP:
        uv_loop_set_data((uv_loop_t *)target, uvdata);
        break;
    case UVDT_HANDLE:
        uv_handle_set_data((uv_handle_t *)target, uvdata);
        break;
    case UVDT_REQ:
        uv_req_set_data((uv_req_t *)target, uvdata);
        break;
    default:
        break;
    }
}

void UVDataHelper::ClearData(void *target, int type)
{
    if (NULL == target)
        return;

    auto loop = GetLoop();
    if (NULL == loop)
    {
        DEBUG("GetLoop get a null value!!!");
        return;
    }

    UVData *uvdata = GetData(target, type);
    if (NULL == uvdata)
        return;
    
    loop->Destroy(uvdata);

    switch (type)
    {
    case UVDT_LOOP:
        uv_loop_set_data((uv_loop_t *)target, NULL);
        break;
    case UVDT_HANDLE:
        uv_handle_set_data((uv_handle_t *)target, NULL);
        break;
    case UVDT_REQ:
        uv_req_set_data((uv_req_t *)target, NULL);
        break;
    default:
        break;
    }
}

UVData *UVDataHelper::GetData(void *target, int type) const
{
    if (NULL == target)
        return NULL;

    switch (type)
    {
    case UVDT_LOOP:
        return (UVData *)uv_loop_get_data((uv_loop_t *)target);
        break;
    case UVDT_HANDLE:
        return (UVData *)uv_handle_get_data((uv_handle_t *)target);
        break;
    case UVDT_REQ:
        return (UVData *)uv_req_get_data((uv_req_t *)target);
        break;
    default:
        break;
    }
    return NULL;
}

void UVDataHelper::BufAlloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    UVData *data = (UVData*)uv_handle_get_data((uv_handle_t*)handle);
    if (NULL == buf || NULL == data)
    {
        buf->base = NULL;
        buf->len = 0;
        return;
    }
    
    UVDataHelper* uvhandle = (UVDataHelper*)data->_self;
    if (NULL == uvhandle)
    {
        DEBUG("self is null!!!");
        return;
    }

    auto loop = uvhandle->GetLoop();
    if (NULL == loop)
    {
        DEBUG("loop is null!!!");
        return;
    }

    buf->base = (char*)loop->Malloc(suggested_size);
    buf->len = suggested_size; 

    DEBUG("OnRead alloc @%p size: %lu\n", buf->base, buf->len);
}

void UVDataHelper::BufFree(const uv_buf_t *buf, UVLoop* loop)
{
    if (NULL == buf || NULL == buf->base || NULL == loop)
    {
        if (loop == NULL)
        {
            DEBUG("******************loop is null!!!******************"); // 内存泄漏了
        }
        return;
    }

    DEBUG("OnRead free @%p\n", buf->base);
    loop->Free(buf->base);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
