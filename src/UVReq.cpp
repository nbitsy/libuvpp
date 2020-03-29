
#include "UVReq.h"
#include "UVLoop.h"

namespace XSpace
{

UVReq::UVReq() : _req(NULL)
{
}

UVReq::~UVReq()
{
}

void UVReq::SetData(void *data, bool force, bool strong)
{
    UVDataHelper::SetData(this, _req, data, force, strong);
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
