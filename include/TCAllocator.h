
#ifndef TCALLOCATOR_H_
#define TCALLOCATOR_H_

#include "tcmalloc/tcmalloc.h"

namespace XNode
{

class TCAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static void *malloc(size_t size) { return tc_newarray_nothrow(size, std::nothrow); }
    static void free(void *p) { return tc_deletearray_nothrow(p, std::nothrow); }
};

} // namespace XNode

#endif // TCALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
