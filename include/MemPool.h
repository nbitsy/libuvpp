
#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <new>      // XXX: for std::ptrdiff_t
#include <stdlib.h> // XXX: for NULL

#include "StdAlloctor.h"
#include "TCAllocator.h"

#include <iostream>

namespace XNode
{

typedef unsigned int MaskType_t;

// 最小块为64字节，8字节对齐
// 64,128,256,512,1k,2k,4k,8k,16k,32k,64k
// > 64k的内存块分配视为大内存块
const int MEMPOOL_SIZE_MIN = 64;        // aligned 8
const int MEMPOOL_SIZE_MAX = 64 * 1024; // aligned 8
const int MEMPOOL_BIN = MEMPOOL_SIZE_MAX / MEMPOOL_SIZE_MIN;

const MaskType_t MEMPOOL_FREE = 0xFEFEFEFE;
const MaskType_t MEMPOOL_USED = 0xDFDFDFDF;

class StdAlloctor;
class TCAlloctor;

// 内存块描述性结构
struct MemPoolBlock
{
    void *_next;
    void *_prev; // store block size when this block is free
    int _size;
    MaskType_t _mask;
    char _data[/*size*/];
    // MaskType_t _mask;
};

#define GET_BEGIN_MASK(block) block->_mask
#define SET_BEGIN_MASK(block, mask) block->_mask = mask
#define SET_END_MASK(block, mask) (*(MaskType_t *)((char *)&block->_data[0] + block->_size) = mask)
#define GET_END_MASK(block) (*(MaskType_t *)((char *)&block->_data[0] + block->_size))

#define SET_FREE(block)                  \
    SET_BEGIN_MASK(block, MEMPOOL_FREE); \
    SET_END_MASK(block, MEMPOOL_FREE)

#define SET_USED(block)                  \
    SET_BEGIN_MASK(block, MEMPOOL_USED); \
    SET_END_MASK(block, MEMPOOL_USED)

#define IS_FREE(block) (GET_BEGIN_MASK(block) == MEMPOOL_FREE && GET_END_MASK(block) == MEMPOOL_FREE)
#define IS_USED(block) (GET_BEGIN_MASK(block) == MEMPOOL_USED && GET_END_MASK(block) == MEMPOOL_USED)

#define BLOCK(p) ((char *)p - &((MemPoolBlock *)0)->_data[0])

template <typename Allocator = StdAlloctor>
class MemPool
{
public:
    MemPool();
    ~MemPool();

    void *AllocBlock(int size);
    void *AddBlock(int size, int idx = -1);
    void FreeBlock(void *p);

    inline bool Empty() const { return _freelist == NULL; }
    inline int Blocks() const { return _blocks; }

    int GetRealSize(int size)
    {
        int realsize = size;
        if (size < MEMPOOL_SIZE_MIN)
        {
            size = MEMPOOL_SIZE_MIN;
            realsize = MEMPOOL_SIZE_MIN;
        }
        else
        {
            realsize = (realsize + (sizeof(void *) - 1)) & ~(sizeof(void *) - 1);
        }

        return realsize;
    }

    // 这个参数是实际分配内存的大小，不包括管理字段的实际大小
    int GetFreeListIdx(int size)
    {
        int idx = 0;
        if (size > MEMPOOL_SIZE_MAX)
            return _freelistcnt;

        while ((size >>= 1) && size >= MEMPOOL_SIZE_MIN)
            ++idx;

        return idx;
    }

private:
    void **_freelist;
    int _freelistcnt;
    int _blocks;
};

template <typename Allocator>
MemPool<Allocator>::MemPool() : _freelist(NULL), _freelistcnt(0), _blocks(0)
{
    int count = 1;
    int maxsize = MEMPOOL_SIZE_MAX;
    int minsize = MEMPOOL_SIZE_MIN;
    while ((maxsize >>= 1) >= minsize)
        ++count;

    _freelistcnt = count;
    _freelist = (void **)Allocator::malloc(count * sizeof(void *));
    memset(_freelist, 0x00, count * sizeof(void *));
}

template <typename Allocator>
MemPool<Allocator>::~MemPool()
{
    if (NULL == _freelist)
        return;

    for (int i = 0; i < _freelistcnt; ++i)
    {
        void *freelist = _freelist[i];
        while (freelist != NULL)
        {
            void *p = freelist;
            freelist = ((MemPoolBlock *)freelist)->_next;
            Allocator::free(p);
        }
    }
}

template <typename Allocator>
void *MemPool<Allocator>::AllocBlock(int size)
{
    int idx = GetFreeListIdx(GetRealSize(size));
    void *freelist = _freelist[idx];
    if (NULL == freelist)
    {
        if (AddBlock(size, idx) == NULL)
            return NULL;
    }

    MemPoolBlock *block = (MemPoolBlock *)_freelist[idx];
    _freelist[idx] = block->_next;
    block->_next = NULL;
    SET_USED(block);
    return &block->_data[0];
}

template <typename Allocator>
void *MemPool<Allocator>::AddBlock(int size, int idx)
{
    int realsize = GetRealSize(size);
    int total = realsize + sizeof(MEMPOOL_FREE) * 2 + sizeof(void *) * 2 + sizeof(int);

    void *p = Allocator::malloc(total); // [NEXT,PREV,realsize[[CHECK][...realsize...][CHECK]]]
    if (NULL == p)
        return NULL;

    if (idx < 0)
        idx = GetFreeListIdx(realsize);

    void *freelist = _freelist[idx];
    MemPoolBlock *block = (MemPoolBlock *)p;
    block->_size = realsize;
    block->_prev = NULL;
    if (NULL == freelist)
    {
        block->_next = NULL;
        _freelist[idx] = p;
        SET_FREE(block);
        return block;
    }

    block->_next = freelist;
    ((MemPoolBlock *)freelist)->_prev = p;
    _freelist[idx] = p;

    ++_blocks;
    return block;
}

template <typename Allocator>
void MemPool<Allocator>::FreeBlock(void *p)
{
    if (NULL == p)
        return;

    MemPoolBlock *block = (MemPoolBlock *)BLOCK(p);
    if (!IS_USED(block))
    {
        std::cout << "Free @" << p << " Error: Double free!!!" << std::endl;
        char *crash = NULL;
        *crash = '0'; // XXX: Crash ^_^
        return;
    }

    block->_next = NULL;
    block->_prev = NULL;

    int idx = GetFreeListIdx(block->_size);
    void *freelist = _freelist[idx];
    if (NULL == freelist)
    {
        _freelist[idx] = block;
    }
    else
    {
        block->_next = freelist;
        _freelist[idx] = block;
    }

    SET_FREE(block);
}

} // namespace XNode

#endif // MEMPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
