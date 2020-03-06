
#ifndef UVHANDLE_H_
#define UVHANDLE_H_

#include "UVDataHelper.h"
#include "UVLoop.h"
#include "uv.h"

namespace XNode
{

class UVHandle : public UVDataHelper
{
public:
    UVHandle();
    virtual ~UVHandle();

    void SetData(void *data, bool force = false);
    const UVData *GetData() const;

    UVLoop* GetLoop() const;
    template <typename T>
    inline T *GetHandle() const { return (T *)_handle; }

    bool IsActive() const;
    bool IsClosing() const;
    void Close();

    int SendBufferSize() const;
    int RecvBufferSize() const;

    int RawFd() const;

    virtual void OnClosed()  = 0;

protected:
    uv_handle_t *_handle;
};

} // namespace XNode

#endif // UVHANDLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
