
#include "UVReq.h"
#include "UVLoop.h"

namespace XSpace
{

UVReq::UVReq(bool gc) : _req(NULL)
{
}

UVReq::~UVReq()
{
}

void UVReq::SetData(void *data, bool force)
{
    UVDataHelper::SetData(_req, data, force);
}

UVData *UVReq::GetData() const
{
    return UVDataHelper::GetData(_req);
}

void UVReq::ClearData()
{
    UVDataHelper::ClearData(_req);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
