
#ifndef _UVREQ_H_
#define _UVREQ_H_

#include <memory>

#include "Config.h"
#include "UVDataHelper.h"

namespace XSpace
{

class UVReq : public UVDataHelper, public std::enable_shared_from_this<UVReq>
{
public:
    WEAK_FROM_THIS(UVReq);
    SHARED_FROM_THIS(UVReq);

public:
    explicit UVReq();
    virtual ~UVReq();

    // strong 强引用加入loop调起后自动回收
    void SetData(void *data, bool force = false, bool strong = true);
    UVData *GetData() const;

    void ClearData();

    template <typename T>
    inline T *GetReq() { return (T *)_req; }

    virtual bool Start() = 0;
    virtual void OnReq(int status) = 0;

protected:
    uv_req_t *_req;
};

} // namespace XSpace

#endif // _UVREQ_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
