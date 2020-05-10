
#ifndef _SHAREDMEMMGR_H_
#define _SHAREDMEMMGR_H_

#include "SharedMem.h"
#include <unordered_map>

namespace XSpace
{

class SharedMemMgr
{
public:
    SharedMemMgr();
    ~SharedMemMgr();

    // 创建一个共享内存块，并返回起始地址
    void* Create(const SharedMemKey_t& key, size_t size, int flag = 0666);
    inline void* CreateOrOpen(const SharedMemKey_t& key, size_t size, int flag = 0666) { return Create(key, size, flag); }
    // 得到映射地址
    void* Get(const SharedMemKey_t& key);
    // 从管理器中删除，内存还是可以使用，但是后面无法索引到
    bool Remove(const SharedMemKey_t& key);

private:
    // 总大小，页对齐总大小
    size_t _size;
    std::unordered_map<SharedMemKey_t, SharedMem*> _sharedMems;
};

} // namespace XSpace

#endif // _SHAREDMEMMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
