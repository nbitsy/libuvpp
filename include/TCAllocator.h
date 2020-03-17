
#ifndef _TCALLOCATOR_H_
#define _TCALLOCATOR_H_

#include "gperftools/tcmalloc.h"

namespace XNode
{

class TCAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    //static void *malloc(size_t size) { return tc_newarray_nothrow(size, std::nothrow); }
    static void *malloc(size_t size) { return tc_malloc(size); }
    static void *calloc(size_t nmemb, size_t size) { return tc_calloc(nmemb, size); }
    static void *realloc(void* p, size_t size) { return tc_realloc(p, size); }
    static void free(void *p) { return tc_free(p); }
    // static void free(void *p) { return tc_deletearray_nothrow(p); }
};

} // namespace XNode

#endif // _TCALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
