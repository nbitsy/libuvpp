
#ifndef _UVPOOLHELPER_H_
#define _UVPOOLHELPER_H_

#include "UVDataHelper.h"
#include "UVReqWrite.h"
#include "Allocator.h"

namespace XSpace
{

class UVPoolHelper
{
public:
    UVPoolHelper() {}
    ~UVPoolHelper() {}

    inline void *Malloc(size_t size)
    {
        return Allocator::malloc(size);
    }

    inline void Free(void *p)
    {
        Allocator::free(p);
    }

    template <typename T, typename... U>
    T *Construct(U... args)
    {
        return Allocator::Construct<T>(args...);
    }

    template <typename T>
    void Destroy(T *p)
    {
        Allocator::Destroy<T>(p);
    }
};

} // namespace XSpace

#endif // _UVPOOLHELPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
