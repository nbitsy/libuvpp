
#ifndef _UVHANDLE_H_
#define _UVHANDLE_H_

#include "uv.h"
#include "UVDataHelper.h"

namespace XNode
{

class UVLoop;

class UVHandle : public UVDataHelper
{
public:
    UVHandle(UVLoop* loop);
    virtual ~UVHandle();

    void SetData(void *data, bool force = false);
    const UVData *GetData() const;
    void ClearData();

    UVLoop *GetLoop() { return _loop; }
    template <typename T>
    inline T *GetHandle() const { return reinterpret_cast<T *>(_handle); }

    bool IsActive() const;
    bool IsClosing() const;
    void Close();

    int SendBufferSize() const;
    int RecvBufferSize() const;

    int RawFd() const;

    /**
     * 完成关闭后调用OnClosed
    */
    virtual void OnClosed() = 0;
    void Release();

protected:
    UVLoop* _loop;
    uv_handle_t *_handle;
};

} // namespace XNode

#endif // _UVHANDLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
