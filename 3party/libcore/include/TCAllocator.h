
#ifndef _TCALLOCATOR_H_
#define _TCALLOCATOR_H_

/**
 * tcmalloc本身就是一个对多线程内存分配支持非常好的内存分配器，并且对于小内存分配的管理也非常棒
 * 所以，没有必要在tcmalloc的基础上去封装内存管理，直接使用tcmalloc进行内存分配即可。
*/

#include "gperftools/tcmalloc.h"

namespace XSpace
{

class TCAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static void *malloc(size_t size) { return tc_malloc(size); }
    static void *calloc(size_t nmemb, size_t size) { return tc_calloc(nmemb, size); }
    static void *realloc(void *p, size_t size) { return tc_realloc(p, size); }
    static void free(void *p) { return tc_free(p); }

    static void *new_(size_t size) { return tc_new(size); }
    static void delete_(void *p) { return tc_delete(p); }
    static void *new_array(size_t size) { return tc_newarray_nothrow(size, std::nothrow); }
    static void delete_array(void *p) { return tc_deletearray_nothrow(p, std::nothrow); }

#if 0
    static void *new_place(size_t size, void *ptr) { return ptr; }
    static void delete_place(void *p, void *ptr) {}
    static void *new_place_array(size_t size, void *ptr) { return ptr; }
    static void delete_place_array(void *p, void *ptr) {}
#endif

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

#if 0
void *operator new(size_t size, void *ptr);
void operator delete(void *p, void *ptr);

void *operator new[](size_t size, void *ptr);
void operator delete[](void *p, void *ptr);
#endif

#endif // _TCALLOCATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
