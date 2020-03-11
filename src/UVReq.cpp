
#include "UVReq.h"
#include "UVLoop.h"

namespace XNode
{

UVReq::UVReq(bool gc) : _req(NULL)
{
}

UVReq::~UVReq()
{
    UVData* uvdata = GetData();
    UVLoop* loop = GetLoop();
    if (uvdata != NULL && loop != NULL)
        loop->Destroy(uvdata);
}

void UVReq::SetData(void *data, bool force)
{
    UVDataHelper::SetData(_req, data, force);
}

UVData *UVReq::GetData() const
{
    return UVDataHelper::GetData(_req);
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
