
#ifndef STDALLOCATOR_H_
#define STDALLOCATOR_H_

#include <stdlib.h> // XXX: for malloc,free
#include <cstdlib> // XXX: for std::size_t
#include <new> // XXX: for std::ptrdiff_t

namespace XNode
{

class StdAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static void *malloc(size_t size) { return ::malloc(size); }
    static void free(void *p) { ::free(p); }
};

} // namespace XNode

#endif // STDALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

