
#ifndef _OBJECTPOOL_H_
#define _OBJECTPOOL_H_

#include "MemPool.h"

namespace XNode
{

/**
 * 缓存对象至少是sizeof(void*)大小，因为当释放对象的时候会加入freelist链表，
 * 这个链表里的所有元素至少需要存放一个void*指令
*/

#define NEXT(cur) *((std::ptrdiff_t *)(cur))

template <typename T, typename Allocator = Allocator>
class ObjectPool : public MemPool<Allocator>
{
public:
    /**
     * size 空闲对象不够时，每次申请对象的个数
    */
    explicit ObjectPool(int size);
    ~ObjectPool();

    template <typename... U>
    T *Construct(U... args)
    {
        void *p = malloc();
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

    void Destroy(T *obj);

    void PrintInfo();

private:
    void *malloc();
    void free(void *p);

private:
    void AllocObjects(int size);

private:
    void *_freelist;
    int _listcount;
    int _allocsize;
};

template <typename T, typename Allocator>
ObjectPool<T, Allocator>::ObjectPool(int size) : _freelist(NULL), _listcount(0), _allocsize(size)
{
    DEBUG("\n");
    AllocObjects(size);
    if (sizeof(T) < sizeof(std::ptrdiff_t))
        std::cerr << "sizeof(T) < " << MEMPOOL_SIZE_MIN << std::endl;
}

template <typename T, typename Allocator>
ObjectPool<T, Allocator>::~ObjectPool()
{
    DEBUG("\n");
    // do nothing
}

template <typename T, typename Allocator>
void ObjectPool<T, Allocator>::AllocObjects(int size)
{
    int needsize = size * sizeof(T);
    void *p = MemPool<Allocator>::AllocBlock(needsize);
    if (p != NULL)
    {
#define OBJECT(p, i) (((char *)(p)) + (sizeof(T) * (i)))
#define LINK(cur, next) (NEXT(cur) = ((std::ptrdiff_t)(next)))

        void *cur = NULL;
        void *next = NULL;
        for (int i = 0; i < size; ++i)
        {
            if (NULL == cur)
                cur = OBJECT(p, i);
            else
                cur = next;

            if (i < size - 1)
            {
                next = OBJECT(p, i + 1);
                LINK(cur, next);
            }
            else
            {
                LINK(cur, _freelist);
            }
        }
    }

    _listcount += size;
    _freelist = p;
}

template <typename T, typename Allocator>
void *ObjectPool<T, Allocator>::malloc()
{
    if (NULL == _freelist)
        AllocObjects(_allocsize);
    if (NULL == _freelist)
        return NULL;

    void *p = _freelist;
    _freelist = (void *)NEXT(_freelist);
    --_listcount;
    return p;
}

template <typename T, typename Allocator>
void ObjectPool<T, Allocator>::free(void *p)
{
    NEXT(p) = (std::ptrdiff_t)_freelist;
    _freelist = p;
    ++_listcount;
}

template <typename T, typename Allocator>
void ObjectPool<T, Allocator>::Destroy(T *obj)
{
    DEBUG("Destroy @%p\n", obj);
    if (obj != NULL)
    {
        obj->~T();
        free(obj);
    }
}

template <typename T, typename Allocator>
void ObjectPool<T, Allocator>::PrintInfo()
{
    MemPool<Allocator>::PrintInfo();
    std::cout << "=====> listcount: " << _listcount << " allocsize: " << _allocsize << std::endl;
}

#undef DEFAULT_ALLOCATOR
#undef FREENEXT

} // namespace XNode

#endif // _OBJECTPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
