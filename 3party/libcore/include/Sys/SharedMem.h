
#ifndef _SHAREDMEM_H_
#define _SHAREDMEM_H_

#include <string>
#ifdef UNIXLIKE
#include <sys/ipc.h>
#endif
#include "Config.h"

#ifdef UNIXLIKE
#ifndef XSPACE_MAP_FILE
#define XSPACE_MAP_FILE "/tmp/xspace_mapping_for_key_wtf_mean"
#endif

#ifndef XSPACE_SHARED_PAGE_SIZE
#define XSPACE_SHARED_PAGE_SIZE (1024 * 4)
#endif

#endif

namespace XSpace
{

#ifdef UNIXLIKE
typedef int SharedMemKey_t;
typedef int SharedMemHandle_t;
#endif
#ifdef WINDOWS
typedef std::string SharedMemKey_t;
typedef HANDLE SharedMemHandle_t;
#endif

class SharedMem
{
public:
    explicit SharedMem(const SharedMemKey_t& key, bool readonly = false, bool destroyWhenDestructor = false);
    ~SharedMem();

    inline void SetReadOnly(bool v = true) { _readOnly = v; }
    inline void SetDestroyWhenDestructor(bool v = true) { _destroyWhenDestructor = v; }

    bool Map(size_t size, int flag = 0666);
    bool Unmap();

    void* operator&() { return _address; }
    const void* operator&() const { return _address; }
    inline size_t Size() const { return _size; }

    // 当没有人引用时就会被删除掉，只要有人还在引用就一直存在
    // 如果调用了这个函数，当没有人映射(attach)共享内存时，会被系统回收，内存中数据全部丢失
    // ！！！危险！！！
    // 同时系统命令: ipcrm 起到的效果跟这个Destroy效果一样
    void Destroy();

private:
    SharedMem(const SharedMem& other);

#ifdef UNIXLIKE
    bool MapUnix(size_t size, int flag);
    bool UnmapUnix();
    void DestroyUnix();
#endif
#ifdef WINDOWS
    bool MapWindows(size_t size, int flag);
    bool UnmapWindows();
    void DestroyWindows();
#endif

private:
    // 共享内存的Key，只要key不一样，创建的共享内存块就不一样
    SharedMemKey_t _key;
    // 是否只读形式映射或者创建
    bool _readOnly;
    // 内存大小 
    size_t _size;
    // 这个实例销毁的时候是否删除共享内存，当删除后，没有其他引用时会释放掉共享内存
    // 在ipcs里也看不见了，如果不删除，则会一直存在，所以引用都退出了也还会存在
    bool _destroyWhenDestructor;
    // 共享内存句柄
    SharedMemHandle_t _handle;
    // 映射后的地址
    void* _address;
};

} // namespace XSpa

#endif // _SHAREDMEM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
