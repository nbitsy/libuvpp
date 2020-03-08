
#include "UVDataHelper.h"

namespace XNode
{

UVData* UVData::Create()
{
    return new UVData(); // TODO:
}

void UVData::Destroy(UVData* uvdata)
{
    if (NULL == uvdata)
        return;
    delete uvdata; // TODO:
}

void UVDataHelper::SetData(void *target, void *data, bool force, int type)
{
    if (NULL == target)
        return;

    UVData *uvdata = GetData(target, type);
    if (NULL == uvdata || force)
    {
        if (uvdata != NULL && !force)
            UVData::Destroy(uvdata);

        uvdata = UVData::Create();
    }

    if (NULL == uvdata)
        return;

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

    UVData *uvdata = GetData(target, type);
    if (NULL == uvdata)
        return;
    
    UVData::Destroy(uvdata);

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

    buf->base = (char *)malloc(suggested_size); // TODO: malloc where
    buf->len = suggested_size; 

    // TODO: test
    std::cout << "OnRead alloc @" << (void*)buf->base << " size: " << buf->len << std::endl;
}

void UVDataHelper::BufFree(const uv_buf_t *buf)
{
    if (NULL == buf || NULL == buf->base)
        return;

    // TODO: test
    std::cout << "OnRead free @" << (void*)buf->base << std::endl;
    free((void*)buf->base); // TODO:
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
