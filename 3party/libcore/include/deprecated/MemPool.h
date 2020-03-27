
#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include <new>      // XXX: for std::ptrdiff_t
#include <stdlib.h> // XXX: for NULL

#include "Allocator.h"
#include "Config.h"
#include "Debugger.h"

#include <iostream>

namespace XSpace
{

// XXX: 如果需要检查内存是否有溢出情况，可以把空上选项打开后编译
#ifndef MEMPOOL_CHECK_OVERFLOW
#define MEMPOOL_CHECK_OVERFLOW
#endif

// TODO: freelist到一定个数或者大小后需要进行释放
// 最小块为64字节，8字节对齐
// 8,16,32,64,128,256,512,1k,2k,4k,8k,16k,32k,64k
// > 64k的内存块分配视为大内存块，直接申请和直接释放
const int MEMPOOL_SIZE_MIN = 8;         // aligned 8
const int MEMPOOL_SIZE_MAX = 64 * 1024; // aligned 8

#ifdef MEMPOOL_CHECK_OVERFLOW
typedef unsigned int MaskType_t;
const MaskType_t MEMPOOL_FREE = 0xFEFEFEFE;
const MaskType_t MEMPOOL_USED = 0xDFDFDFDF;
#endif

#pragma pack(1)
// 内存块描述性结构
struct MemPoolBlock
{
    union {
        struct
        {
#ifdef LITTLE_ENDIAN
            int _freed; // 是否使用中: 0-using >address or 1 -free
            int _size;  // 内存块使用中时完成里存放的是块大小
#else
            int _size;  // 内存块使用中时完成里存放的是块大小
            int _freed; // 是否使用中: 0-using >address or 1 -free
#endif
        };
        void *_next;
    };
#ifdef MEMPOOL_CHECK_OVERFLOW
    MaskType_t _mask;
#endif
    char _data[]; // ... Body
};
#pragma pack()

struct MemPoolBin
{
    void *_binlist;
    int _count;
};

#ifdef MEMPOOL_CHECK_OVERFLOW
#define HEAD_SIZE() (sizeof(MemPoolBlock) + sizeof(MaskType_t))
#define GET_BEGIN_MASK(block) block->_mask
#define SET_BEGIN_MASK(block, mask) block->_mask = (mask)

#define SET_END_MASK(block, mask, binsize) (*(MaskType_t *)((((char *)(block)) + (binsize)) - sizeof(MaskType_t))) = (mask)
#define GET_END_MASK(block, binsize) (*(MaskType_t *)((((char *)(block)) + (binsize)) - sizeof(MaskType_t)))

#define SET_FREE(block, size)            \
    SET_BEGIN_MASK(block, MEMPOOL_FREE); \
    SET_END_MASK(block, MEMPOOL_FREE, size)

#define SET_USED(block, size)            \
    SET_BEGIN_MASK(block, MEMPOOL_USED); \
    SET_END_MASK(block, MEMPOOL_USED, size)

#define IS_FREE(block, size) ((GET_BEGIN_MASK(block) == MEMPOOL_FREE) && (GET_END_MASK(block, size) == MEMPOOL_FREE))
#define IS_USED(block, size) ((GET_BEGIN_MASK(block) == MEMPOOL_USED) && (GET_END_MASK(block, size) == MEMPOOL_USED))
#else
#define HEAD_SIZE() (sizeof(MemPoolBlock))
#define SET_USED(...)
#define SET_FREE(...)
#define IS_FREE(...)
#define IS_USED(...) true
#endif

#define LIST_EMPTY(list) (NULL == (list) || ((std::ptrdiff_t)(list)) == 0x1)
#define IS_NULL(block) (NULL == (block) || ((std::ptrdiff_t)(block)) == 0x1)

#define BLOCK(p) ((char *)p - &((MemPoolBlock *)0)->_data[0])

const int MEMPOOL_USABLE_SIZE_MAX = MEMPOOL_SIZE_MAX - HEAD_SIZE();

template <typename Allocator = Allocator>
class MemPool
{
public:
    MemPool();
    ~MemPool();

    void *AllocBlock(int size);
    void FreeBlock(void *p);
    inline bool Empty() const { return _binlist == NULL || 0 == _blocks; }
    inline int Blocks() const { return _blocks; }

    // TODO: 加速这个过程
    // size 整个块的大小，所以包括头部大小
    // 这个参数是实际分配内存的大小，不包括管理字段的实际大小
    int GetFreeListIdx(int size, int &binsize, const char *reason)
    {
        int bsize = MEMPOOL_SIZE_MIN;
        int idxsize = size;
        int idx = 0;

        if (size - HEAD_SIZE() > MEMPOOL_USABLE_SIZE_MAX)
        {
            ERROR("size: %d idx: %d\n", size, _binlistcnt - 1);
            return _binlistcnt - 1;
        }

        while (idxsize > MEMPOOL_SIZE_MIN)
        {
            ++idx;
            idxsize >>= 1;
            bsize <<= 1;
        }

        if (bsize < size)
        {
            idx++;
            bsize <<= 1;
        }

        binsize = bsize;
        DEBUG("[%s} size: %d idx: %d binsize: %d\n", reason, size, idx, binsize);
        return idx;
    }

    void PrintInfo();

private:
    void *AllocBigBlock(int size);
    void FreeBigBlock(void *p);
    void *AddBlock(int binsize, int idx = -1);

private:
    MemPoolBin *_binlist;
    int _binlistcnt;
    int _blocks;
    int _blocksalloced;
};

template <typename Allocator>
MemPool<Allocator>::MemPool() : _binlist(NULL), _binlistcnt(0), _blocks(0), _blocksalloced(0)
{
    int count = 1; // 至少保证最小块
    int maxsize = MEMPOOL_SIZE_MAX;
    int minsize = MEMPOOL_SIZE_MIN;
    while ((maxsize >>= 1) >= minsize)
        ++count;

    _binlistcnt = count;
    _binlist = (MemPoolBin *)Allocator::malloc(count * sizeof(MemPoolBin));
    if (_binlist != NULL)
        memset(_binlist, 0x00, count * sizeof(MemPoolBin));
}

template <typename Allocator>
MemPool<Allocator>::~MemPool()
{
    if (NULL == _binlist)
        return;

    for (int i = 0; i < _binlistcnt; ++i)
    {
        auto &freelist = _binlist[i]._binlist;
        while (!LIST_EMPTY(freelist))
        {
            void *p = freelist;
            freelist = ((MemPoolBlock *)freelist)->_next;
            DEBUG(" Free@ %p\n", p);
            Allocator::free(p);
        }
    }

    //PrintInfo();
    if (_binlist != NULL)
        Allocator::free(_binlist);
}

template <typename Allocator>
void *MemPool<Allocator>::AllocBigBlock(int size)
{
    if (size <= 0)
        size = MEMPOOL_SIZE_MIN;

    int binszie = size + HEAD_SIZE();
    binszie = (binszie + (sizeof(void *) - 1)) & ~(sizeof(void *) - 1);

    MemPoolBlock *block = (MemPoolBlock *)Allocator::malloc(binszie);
    block->_size = binszie;
    block->_freed = 0;
    SET_USED(block, binszie);
    DEBUG("AllocBigBlock @%p realaddr: @%p with size: %d realsize: %d\n", &block->_data[0], block, size, block->_size);
    return &block->_data[0];
}

template <typename Allocator>
void MemPool<Allocator>::FreeBigBlock(void *p)
{
    MemPoolBlock *block = (MemPoolBlock *)BLOCK(p);
    DEBUG("FreeBigBlock@ %p realaddr: %p with size: %d\n", p, block, block->_size);

#ifdef MEMPOOL_CHECK_OVERFLOW
    int binsize = block->_size;
#endif
    int freed = block->_freed;
    if ((!freed && !IS_USED(block, binsize)) || freed)
    {
        ERROR("Free @%p Error: Double free!!!\n", p);
#ifdef _DEBUG
        char *crash = NULL;
        *crash = '0'; // XXX: Crash ^_^
#endif
        return;
    }

    Allocator::free(block); // 直接释放
}

template <typename Allocator>
void *MemPool<Allocator>::AllocBlock(int size)
{
    if (NULL == _binlist)
        return NULL;

#ifdef _DEBUG
    if (size < MEMPOOL_SIZE_MIN)
        WARN("Alloc size is too small: %d\n", size);
#endif

    if (size > MEMPOOL_USABLE_SIZE_MAX)
        return AllocBigBlock(size);

    int binsize = 0;
    int idx = GetFreeListIdx(size + HEAD_SIZE(), binsize, "ALLOC");

    auto &memblockbin = _binlist[idx];
    auto &freelist = memblockbin._binlist;

    MemPoolBlock *block = NULL;
    if (LIST_EMPTY(freelist))
        block = (MemPoolBlock *)AddBlock(binsize, idx);
    else
        block = (MemPoolBlock *)freelist;

    if (IS_NULL(block))
        return NULL;

    freelist = block->_next;

    block->_next = NULL;
    block->_freed = 0;
    block->_size = binsize; // 使用中时_next里才存放size和使用中标志
    SET_USED(block, binsize);

    --memblockbin._count;
    --_blocks;

    DEBUG("AllocBlock @%p realaddr: @%p with size: %d realsize: %d\n", &block->_data[0], block, size, binsize);
    return &block->_data[0];
}

template <typename Allocator>
void *MemPool<Allocator>::AddBlock(int binsize, int idx)
{
    if (NULL == _binlist)
        return NULL;

    void *p = Allocator::malloc(binsize); // [NEXT,[[CHECK][...binsize-HEAD_SIZE()...][CHECK]]]
    if (NULL == p)
        return NULL;

    DEBUG("AddBlock @%p binsize: %d\n", p, binsize);
    auto &memblockbin = _binlist[idx];
    auto &freelist = memblockbin._binlist;

    ++memblockbin._count;
    ++_blocks;
    ++_blocksalloced;

    MemPoolBlock *block = (MemPoolBlock *)p;
    SET_FREE(block, binsize);
    if (LIST_EMPTY(freelist))
    {
        block->_next = NULL;
        block->_freed = 1;
        freelist = block;
        return block;
    }

    block->_next = freelist;
    freelist = block;
    return block;
}

template <typename Allocator>
void MemPool<Allocator>::FreeBlock(void *p)
{
    if (NULL == _binlist)
        return;

    if (NULL == p)
        return;

    MemPoolBlock *block = (MemPoolBlock *)BLOCK(p);
    int size = block->_size;
    if (size > MEMPOOL_USABLE_SIZE_MAX)
    {
        FreeBigBlock(p);
        return;
    }

    int freed = block->_freed;
    if ((!freed && !IS_USED(block, size)) || freed)
    {
        ERROR("Free @%p Error: Double free or overflow!!!\n", p);
#ifdef _DEBUG
        char *crash = NULL;
        *crash = '0'; // XXX: Crash ^_^
#endif
        return;
    }

    int binsize = 0;
    int idx = GetFreeListIdx(size, binsize, "FREE");
    DEBUG("FreeBlock@ %p realaddr: %p with size: %d\n", p, block, binsize);

    auto &memblockbin = _binlist[idx];
    auto &freelist = memblockbin._binlist;
    block->_size = 0;
    block->_freed = 1; // 0x0000000000000001 也为空

    if (LIST_EMPTY(freelist))
    {
        freelist = block;
    }
    else
    {
        block->_next = freelist; // over write _freed
        freelist = block;
    }

    ++memblockbin._count;
    ++_blocks;
    SET_FREE(block, binsize);
}

template <typename Allocator>
void MemPool<Allocator>::PrintInfo()
{
    std::cout << "======================MemPoolInfo=====================" << std::endl;
    std::cout << "blocks: " << Blocks() << " alloced: " << _blocksalloced << std::endl;
    for (int i = 0; i < _binlistcnt; ++i)
    {
        auto &memblockbin = _binlist[i];
        if (i == (_binlistcnt - 1))
            std::cout << "  MemPoolBin(>" << 64 * (1 << (i - 1)) << ") blocks: " << memblockbin._count << std::endl;
        else
            std::cout << "  MemPoolBin(<=" << 64 * (1 << i) << ") blocks: " << memblockbin._count << std::endl;
    }
    std::cout << "======================MemPoolInfo=====================" << std::endl;
}

} // namespace XSpace

#endif // _MEMPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
