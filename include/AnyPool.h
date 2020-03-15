
#ifndef _ANYPOOL_H_
#define _ANYPOOL_H_

#include "Allocator.h"
#include "MemPool.h"

namespace XNode
{

class AnyPool : public MemPool<Allocator>
{
public:
    AnyPool() {}
    virtual ~AnyPool() {}

    template <typename T, typename... U>
    T *Construct(U... args)
    {
        void *p = malloc(sizeof(T));
        if (NULL == p)
            return NULL;

        DEBUG("Construct @%p\n", p);
        try
        {
            return new (p) T(args...);
        }
        catch (...)
        {
            free(p);
            return NULL;
        }

        return NULL;
    }

    template <typename T>
    void Destroy(T *obj) { free(obj); }

private:
    inline void *malloc(size_t size) { return AllocBlock(size); }
    inline void free(void *p) { FreeBlock(p); }
};

} // namespace XNode

#endif // _ANYPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
