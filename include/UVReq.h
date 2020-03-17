
#ifndef _UVREQ_H_
#define _UVREQ_H_

#include "UVDataHelper.h"

namespace XNode
{

class UVReq : public UVDataHelper
{
public:
    explicit UVReq(bool gc = true);
    virtual ~UVReq();

    void SetData(void *data, bool force = false);
    UVData *GetData() const;

    void ClearData();

    template <typename T>
    inline T *GetReq() { return (T *)_req; }

    virtual UVLoop* GetLoop() { return NULL; }

    virtual bool Start() = 0;
    virtual void OnReq(int status) = 0;

protected:
    uv_req_t *_req;
};

} // namespace XNode

#endif // _UVREQ_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
