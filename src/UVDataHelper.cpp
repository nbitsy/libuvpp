
#include "UVDataHelper.h"

namespace XNode
{

void UVDataHelper::SetData(void *target, void *data, bool force, int type)
{
    if (NULL == target)
        return;

    UVData *pdata = (UVData *)GetData(target, type);
    if (NULL == pdata || force)
    {
        if (pdata != NULL && !force)
            delete pdata; // TODO: 

        pdata = new UVData(); // TODO:
    }

    if (NULL == pdata)
        return;

    pdata->_self = this;
    pdata->_data = data;

    switch (type)
    {
    case UVDT_LOOP:
        uv_loop_set_data((uv_loop_t *)target, pdata);
        break;
    case UVDT_HANDLE:
        uv_handle_set_data((uv_handle_t *)target, pdata);
        break;
    case UVDT_REQ:
        uv_req_set_data((uv_req_t *)target, pdata);
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

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
