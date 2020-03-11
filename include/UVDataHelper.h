
#ifndef _UVDATAHELPER_H_
#define _UVDATAHELPER_H_

#include "Debuger.h"
#include "ObjectPool.h"
#include "uv.h"

namespace XNode
{

enum UVDataType
{
    UVDT_LOOP = 0,
    UVDT_HANDLE,
    UVDT_REQ,
    UVDT_OTH,
};

class UVLoop;

// TODO：对象池
struct UVData
{
    UVData() : _self(NULL), _data(NULL) {}
    UVData(void *self, void *data) : _self(self), _data(data) {}

    ~UVData()
    {
        DEBUG("_self: %p, _data: %p\n", _self, _data);
    }

    void *_self; // 本对象的原始对象
    void *_data; // 外部数据
};

class UVDataHelper
{
public:
    UVDataHelper() : _bgc(true) {}
    virtual ~UVDataHelper() {}

    /**
     * target 设置数据的目标对象
     * data 需要设置的数据
     * force 是否强制设置，不管对象是否已经设置过数据
    */
    template <typename T>
    void SetData(T *target, void *data, bool force);

    inline void SetData(uv_loop_t *target, void *data, bool force)
    {
        SetData(target, data, force, UVDT_LOOP);
    }
    inline void SetData(uv_handle_t *target, void *data, bool force)
    {
        SetData(target, data, force, UVDT_HANDLE);
    }
    inline void SetData(uv_req_t *target, void *data, bool force)
    {
        SetData(target, data, force, UVDT_REQ);
    }

    template <typename T>
    void ClearData(T *target);

    inline void ClearData(uv_loop_t *target)
    {
        ClearData(target, UVDT_LOOP);
    }
    inline void ClearData(uv_handle_t *target)
    {
        ClearData(target, UVDT_HANDLE);
    }
    inline void ClearData(uv_req_t *target)
    {
        ClearData(target, UVDT_REQ);
    }

    template <typename T>
    UVData *GetData(T *target) const;

    inline UVData *GetData(uv_loop_t *target) const
    {
        return GetData(target, UVDT_LOOP);
    }
    inline UVData *GetData(uv_handle_t *target) const
    {
        return GetData(target, UVDT_HANDLE);
    }
    inline UVData *GetData(uv_req_t *target) const
    {
        return GetData(target, UVDT_REQ);
    }

    virtual UVLoop *GetLoop() = 0;
    /**
     * Release里干的是一些释放资源的工作，包括对象自己，因为进入UVLoop的对象已经不受自己控制了
    */
    virtual void Release() = 0;

    inline void SetGC(bool gc = true) { _bgc = gc; }
    inline bool GetGC() const { return _bgc; }

    static void BufAlloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void BufFree(const uv_buf_t *buf, UVLoop *loop);

    template <typename T>
    static T *HandleAlloc();
    static void HandleFree(uv_handle_t *handle);

    template <typename T>
    static T *ReqAlloc();
    static void ReqFree(uv_req_t *req);

private:
    void SetData(void *target, void *data, bool force, int type);
    void ClearData(void *target, int type);
    UVData *GetData(void *target, int type) const;

protected:
    bool _bgc;
};

} // namespace XNode

#endif // _UVDATAHELPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
