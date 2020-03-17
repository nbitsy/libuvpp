
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#ifdef USE_TC_MALLOC
#include "TCAllocator.h"
#else
#include "StdAllocator.h"
#endif

namespace XNode
{
#ifdef USE_TC_MALLOC
typedef TCAllocator Allocator;
#else
typedef StdAllocator Allocator;
#endif
} // namespace XNode

#endif // _ALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

