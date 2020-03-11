
#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <new>      // XXX: for std::ptrdiff_t
#include <stdlib.h> // XXX: for NULL

#include "Allocator.h"
#include "Debuger.h"

#include <iostream>

namespace XNode
{

typedef unsigned int MaskType_t;

// TODO: freelist到一定个数或者大小后需要进行释放
// 最小块为64字节，8字节对齐
// 64,128,256,512,1k,2k,4k,8k,16k,32k,64k
// > 64k的内存块分配视为大内存块
const int MEMPOOL_SIZE_MIN = 64;        // aligned 8
const int MEMPOOL_SIZE_MAX = 64 * 1024; // aligned 8

const MaskType_t MEMPOOL_FREE = 0xFEFEFEFE;
const MaskType_t MEMPOOL_USED = 0xDFDFDFDF;

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

struct MemPoolBin
{
    void *_freelist;
    int _count;
};

#define GET_BEGIN_MASK(block) block->_mask
#define SET_BEGIN_MASK(block, mask) block->_mask = (mask)
#define SET_END_MASK(block, mask) (*(MaskType_t *)((char *)&block->_data[0] + block->_size) = (mask))
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

template <typename Allocator = Allocator>
class MemPool
{
public:
    MemPool();
    ~MemPool();

    void *AllocBlock(int size);
    void FreeBlock(void *p);

    inline bool Empty() const { return _freelist == NULL || 0 == _blocks; }
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
        int idxsize = size;
        int idx = 0;
        if (idxsize > MEMPOOL_SIZE_MAX)
        {
            DEBUG("size: %d idx: %d\n", size, _freelistcnt - 1);
            return _freelistcnt - 1;
        }

        while (idxsize > MEMPOOL_SIZE_MIN)
        {
            ++idx;
            idxsize >>= 1;
        }

        DEBUG("size: %d idx: %d\n", size, idx);
        return idx;
    }

    void PrintInfo();

private:
    void *AddBlock(int size, int idx = -1);

private:
    MemPoolBin *_freelist;
    int _freelistcnt;
    int _blocks;
    int _blocksalloced;
};

template <typename Allocator>
MemPool<Allocator>::MemPool() : _freelist(NULL), _freelistcnt(0), _blocks(0), _blocksalloced(0)
{
    int count = 2; // 至少保证最小块和最大块链
    int maxsize = MEMPOOL_SIZE_MAX;
    int minsize = MEMPOOL_SIZE_MIN;
    while ((maxsize >>= 1) >= minsize)
        ++count;

    _freelistcnt = count;
    _freelist = (MemPoolBin *)Allocator::malloc(count * sizeof(MemPoolBin));
    if (_freelist != NULL)
        memset(_freelist, 0x00, count * sizeof(MemPoolBin));
}

template <typename Allocator>
MemPool<Allocator>::~MemPool()
{
    if (NULL == _freelist)
        return;

    for (int i = 0; i < _freelistcnt; ++i)
    {
        auto &freelist = _freelist[i]._freelist;
        while (freelist != NULL)
        {
            void *p = freelist;
            freelist = ((MemPoolBlock *)freelist)->_next;
            DEBUG(" Free@ %p\n", p);
            Allocator::free(p);
        }
    }

    if (_freelist != NULL)
        Allocator::free(_freelist);
}

template <typename Allocator>
void *MemPool<Allocator>::AllocBlock(int size)
{
    if (NULL == _freelist)
        return NULL;

#ifdef _DEBUG
    if (size < MEMPOOL_SIZE_MIN)
        WARN("Alloc size is too small: %d\n", size);
#endif

    int idx = GetFreeListIdx(GetRealSize(size));
    auto &memblockbin = _freelist[idx];
    auto &freelist = memblockbin._freelist;
    if (NULL == freelist)
    {
        if (AddBlock(size, idx) == NULL)
            return NULL;
    }

    MemPoolBlock *block = (MemPoolBlock *)freelist;
    freelist = block->_next;
    block->_next = NULL;
    block->_prev = NULL;
    SET_USED(block);

    --memblockbin._count;
    --_blocks;
    return &block->_data[0];
}

template <typename Allocator>
void *MemPool<Allocator>::AddBlock(int size, int idx)
{
    if (NULL == _freelist)
        return NULL;

    int realsize = GetRealSize(size);
    int total = realsize + sizeof(MEMPOOL_FREE) * 2 + sizeof(void *) * 2 + sizeof(int);

    void *p = Allocator::malloc(total); // [NEXT,PREV,realsize[[CHECK][...realsize...][CHECK]]]
    if (NULL == p)
        return NULL;

    if (idx < 0)
        idx = GetFreeListIdx(realsize);

    DEBUG("AddBlock @%p with size: %d => realsize: %d total: %d\n", p, size, realsize, total);
    auto &memblockbin = _freelist[idx];
    auto &freelist = memblockbin._freelist;

    MemPoolBlock *block = (MemPoolBlock *)p;
    block->_size = realsize;
    block->_prev = NULL;

    ++memblockbin._count;
    ++_blocks;
    ++_blocksalloced;

    if (NULL == freelist)
    {
        block->_next = NULL;
        freelist = p;
        SET_FREE(block);
        return block;
    }

    block->_next = freelist;
    ((MemPoolBlock *)freelist)->_prev = p;
    freelist = p;

    return block;
}

template <typename Allocator>
void MemPool<Allocator>::FreeBlock(void *p)
{
    if (NULL == _freelist)
        return;

    if (NULL == p)
        return;

    MemPoolBlock *block = (MemPoolBlock *)BLOCK(p);
    if (!IS_USED(block))
    {
        ERROR("Free @%p Error: Double free!!!\n", p);
#ifdef _DEBUG
        char *crash = NULL;
        *crash = '0'; // XXX: Crash ^_^
#endif
        return;
    }

    DEBUG("FreeBlock@ %p realaddr: %p with size: %d\n", p, block, block->_size);
    int idx = GetFreeListIdx(block->_size);
    auto &memblockbin = _freelist[idx];
    auto &freelist = memblockbin._freelist;
    if (NULL == freelist)
    {
        freelist = block;
    }
    else
    {
        block->_next = freelist;
        freelist = block;
    }

    ++memblockbin._count;
    ++_blocks;
    SET_FREE(block);
}
template <typename Allocator>
void MemPool<Allocator>::PrintInfo()
{
    std::cout << "======================MemPoolInfo=====================" << std::endl;
    std::cout << "blocks: " << Blocks() << " alloced: " << _blocksalloced << std::endl;
    for (int i = 0; i < _freelistcnt; ++i)
    {
        auto &memblockbin = _freelist[i];
        if (i == (_freelistcnt - 1))
            std::cout << "  MemPoolBin(>" << 64 * (1 << (i - 1)) << ") blocks: " << memblockbin._count << std::endl;
        else
            std::cout << "  MemPoolBin(<=" << 64 * (1 << i) << ") blocks: " << memblockbin._count << std::endl;
    }
    std::cout << "======================MemPoolInfo=====================" << std::endl;
}

} // namespace XNode

#endif // MEMPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
