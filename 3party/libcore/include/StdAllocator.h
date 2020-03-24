
#ifndef _STDALLOCATOR_H_
#define _STDALLOCATOR_H_

#include <cstdlib>  // XXX: for std::size_t
#include <new>      // XXX: for std::ptrdiff_t
#include <stdlib.h> // XXX: for malloc,free

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
    static void *realloc(void *p, size_t size) { return ::realloc(p, size); }
    static void free(void *p) { return ::free(p); }

    static void *new_array(size_t size) { return ::malloc(size); }
    static void delete_array(void *p) { return ::free(p); }
    static void *new_(size_t size) { return ::malloc(size); }
    static void delete_(void *p) { return ::free(p); }

public:
    template <typename T, typename... U>
    static T *Construct(U... args)
    {
        void *p = malloc(sizeof(T));
        if (NULL == p)
            return NULL;

        try
        {
            return ::new (p) T(args...);
        }
        catch (...)
        {
            free(p);
            return NULL;
        }

        return NULL;
    }

    template <typename T>
    static void Destroy(T *obj)
    {
        if (obj != NULL)
        {
            obj->~T();
            free(obj);
        }
    }
};

} // namespace XSpace

void *operator new(size_t size);
void operator delete(void *p) noexcept;

void *operator new[](size_t size);
void operator delete[](void *p) noexcept;

#endif // _STDALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
