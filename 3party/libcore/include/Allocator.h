
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#ifdef USE_TC_MALLOC
#include "TCAllocator.h"
#else
#include "StdAllocator.h"
#endif

namespace XSpace
{
#ifdef USE_TC_MALLOC
typedef TCAllocator Allocator;
#else
typedef StdAllocator Allocator;
#endif

template <typename T>
struct Deleter
{
    inline void operator()(T* i) { Allocator::Destroy(i); }
    inline void operator()(const T* i) { Allocator::Destroy(i); }
};

} // namespace XSpace

#endif // _ALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

