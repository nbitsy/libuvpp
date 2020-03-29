
#ifndef _UVHANDLE_H_
#define _UVHANDLE_H_

#include <memory>

#include "Config.h"
#include "UVDataHelper.h"
#include "uv.h"

namespace XSpace
{

class UVLoop;

class UVHandle : public UVDataHelper, public std::enable_shared_from_this<UVHandle>
{
public:
    UVHandle(std::weak_ptr<UVLoop> &loop);
    virtual ~UVHandle();

    void SetData(void *data, bool force = false, bool strong = false);
    const UVData *GetData() const;
    void ClearData();

    std::weak_ptr<UVLoop> &GetLoop() { return _loop; }
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

protected:
    std::weak_ptr<UVLoop> _loop;
    uv_handle_t *_handle;
};

} // namespace XSpace

#endif // _UVHANDLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
