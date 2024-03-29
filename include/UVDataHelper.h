
#ifndef _UVDATAHELPER_H_
#define _UVDATAHELPER_H_

#include <memory>

#include "Allocator.h"
#include "Debugger.h"
#include "TypeTraits.h"
#include "uv.h"

namespace XSpace
{

// TODO: 如果在创建的时候有更多的定制性需求，需要考虑把SetData独立出来
// XXX: 所有Handle和UVLoop都是弱引用的，外部需要强引用管理，而Req一般都是弱引用的，OnReq后需要回收
// XXX: 所有继承自UVHandle的类需要提供以loop为第一参数的构造函数
#define UV_CREATE_HANDLE_CREATE(TYPE)                                                   \
    template <typename T = TYPE, typename L = UVLoop, typename... U>                    \
    inline static std::shared_ptr<TYPE> Create(const std::weak_ptr<L>& loop, U... args) \
    {                                                                                   \
        return CreateWeak<T, L, U...>(loop, args...);                                   \
    }

#define UV_CREATE_HANDLE_(TYPE, SHARED, WEAK)                                                   \
    template <typename T = TYPE, typename L = UVLoop, typename... U>                            \
    static std::shared_ptr<TYPE> Create##WEAK(const std::weak_ptr<L>& loop, U... args)          \
    {                                                                                           \
        if (is_subclass<T, TYPE>::value)                                                        \
        {                                                                                       \
            std::shared_ptr<TYPE> ptr(Allocator::Construct<T>(loop, args...), Deleter<TYPE>()); \
            if (ptr != NULL)                                                                    \
                ptr->SetData(NULL, true, SHARED);                                               \
                                                                                                \
            return ptr;                                                                         \
        }                                                                                       \
                                                                                                \
        return NULL;                                                                            \
    }

#define UV_CREATE_REQ(TYPE, SHARED)                                                       \
    template <typename T = TYPE, typename... U>                                           \
    static std::shared_ptr<TYPE> Create(U... args)                                        \
    {                                                                                     \
        if (is_subclass<T, TYPE>::value)                                                  \
        {                                                                                 \
            std::shared_ptr<TYPE> ptr(Allocator::Construct<T>(args...), Deleter<TYPE>()); \
            if (ptr != NULL)                                                              \
                ptr->SetData(NULL, true, SHARED);                                         \
                                                                                          \
            return ptr;                                                                   \
        }                                                                                 \
                                                                                          \
        return NULL;                                                                      \
    }

#define UV_CREATE_HANDLE_WEAK(TYPE)   UV_CREATE_HANDLE_(TYPE, false, Weak)
#define UV_CREATE_HANDLE_SHARED(TYPE) UV_CREATE_HANDLE_(TYPE, true, Shared)

#define UV_CREATE_HANDLE(TYPE)    \
    UV_CREATE_HANDLE_CREATE(TYPE) \
    UV_CREATE_HANDLE_WEAK(TYPE)   \
    UV_CREATE_HANDLE_SHARED(TYPE)

#define UV_CREATE_REQ_WEAK(TYPE)   UV_CREATE_REQ(TYPE, false)
#define UV_CREATE_REQ_SHARED(TYPE) UV_CREATE_REQ(TYPE, true)

enum UVDataType
{
    UVDT_LOOP = 0,
    UVDT_HANDLE,
    UVDT_REQ,
    UVDT_OTH,
};

class UVLoop;

struct UVData
{
    UVData() : _strong(false), _self(NULL), _data(NULL) {}
    UVData(bool strong, void* self, void* data);
    ~UVData();

    template <typename T>
    inline std::weak_ptr<T>* GetWeakPtr()
    {
        if (_strong)
            return NULL;
        return (std::weak_ptr<T>*)_self;
    }

    template <typename T>
    inline std::shared_ptr<T>* GetSharedPtr()
    {
        if (!_strong)
            return NULL;
        return (std::shared_ptr<T>*)_self;
    }

    template <typename T>
    T* GetPtr()
    {
        T* t = NULL;
        if (_strong)
        {
            auto self = GetSharedPtr<T>();
            t = self->get();
        }
        else
        {
            auto self = GetWeakPtr<T>();
            if (!self->expired())
                t = self->lock().get();
        }
        return t;
    }

    bool _strong; // _self是一个强引用
    void* _self;  // 本对象的weak_ptr指针
    void* _data;  // 外部数据
};

class UVDataHelper
{
public:
    UVDataHelper() {}
    virtual ~UVDataHelper() {}

    template <typename T, typename U>
    void SetData(T* object, U* target, void* data, bool force, bool strong);

    template <typename T>
    inline void SetData(T* object, uv_loop_t* target, void* data, bool force, bool strong)
    {
        SetData(object, target, data, force, UVDT_LOOP, strong);
    }
    template <typename T>
    inline void SetData(T* object, uv_handle_t* target, void* data, bool force, bool strong)
    {
        SetData(object, target, data, force, UVDT_HANDLE, strong);
    }
    template <typename T>
    inline void SetData(T* object, uv_req_t* target, void* data, bool force, bool strong)
    {
        SetData(object, target, data, force, UVDT_REQ, strong);
    }

    template <typename T>
    void ClearData(T* target);

    inline void ClearData(uv_loop_t* target)
    {
        ClearData(target, UVDT_LOOP);
    }
    inline void ClearData(uv_handle_t* target)
    {
        ClearData(target, UVDT_HANDLE);
    }
    inline void ClearData(uv_req_t* target)
    {
        ClearData(target, UVDT_REQ);
    }

    template <typename T>
    UVData* GetData(T* target) const;

    inline UVData* GetData(uv_loop_t* target) const
    {
        return GetData(target, UVDT_LOOP);
    }
    inline UVData* GetData(uv_handle_t* target) const
    {
        return GetData(target, UVDT_HANDLE);
    }
    inline UVData* GetData(uv_req_t* target) const
    {
        return GetData(target, UVDT_REQ);
    }

    static void BufAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    static void BufFree(const uv_buf_t* buf);

private:
    /**
     * object 目标对象对象的C++包装
     * target 设置数据的目标对象
     * data 需要设置的数据
     * force 是否强制设置，不管对象是否已经设置过数据
     * type 对象类型
     * strong 对于object是否强引用
    */
    template <typename T>
    void SetData(T* object, void* target, void* data, bool force, int type, bool strong)
    {
        if (NULL == target)
            return;

        void* self = NULL;
        if (strong)
            self = Allocator::Construct<std::shared_ptr<UVDataHelper>>(object->shared_from_this());
        else
            self = Allocator::Construct<std::weak_ptr<UVDataHelper>>(object->shared_from_this());

        if (NULL == self)
            return;

        UVData* uvdata = GetData(target, type);
        if (uvdata != NULL && force)
            Allocator::Destroy(uvdata);

        uvdata = Allocator::Construct<UVData>(strong, self, data);
        if (NULL == uvdata)
        {
            ERROR("ERROR: alloc UVData!!!\n");
            return;
        }

        switch (type)
        {
        case UVDT_LOOP:
            uv_loop_set_data((uv_loop_t*)target, uvdata);
            break;
        case UVDT_HANDLE:
            uv_handle_set_data((uv_handle_t*)target, uvdata);
            break;
        case UVDT_REQ:
            uv_req_set_data((uv_req_t*)target, uvdata);
            break;
        default:
            break;
        }
    }

    void ClearData(void* target, int type);
    UVData* GetData(void* target, int type) const;
};

} // namespace XSpace

#endif // _UVDATAHELPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
