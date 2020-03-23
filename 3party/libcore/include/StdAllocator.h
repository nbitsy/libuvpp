
#ifndef _STDALLOCATOR_H_
#define _STDALLOCATOR_H_

#include <stdlib.h> // XXX: for malloc,free
#include <cstdlib> // XXX: for std::size_t
#include <new> // XXX: for std::ptrdiff_t

namespace XSpace
{

class StdAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static void *malloc(size_t size) { return ::malloc(size); }
    static void *calloc(size_t nmemb, size_t size) { return ::calloc(nmemb, size); }
    static void *realloc(void* p, size_t size) { return ::realloc(p, size); }
    static void free(void *p) { return ::free(p); }
};

} // namespace XSpace

#endif // _STDALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
