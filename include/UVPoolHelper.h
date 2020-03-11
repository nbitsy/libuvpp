
#ifndef UVPOOLHELPER_H_
#define UVPOOLHELPER_H_

#include "ObjectPool.h"
#include "UVDataHelper.h"
#include "UVReqWrite.h"

namespace XNode
{

class UVPoolHelper
{
public:
    UVPoolHelper()
        // data
        : _data(100)
          // cpp req
          , _reqwrite(100)
          // uv req
          , _uvconnect(100), _uvwrite(10), _uvudpsend(10)
          // uv handle
          , _uvtcp(50), _uvudp(10), _uvtimer(100), _uvasync(10)
    {
    }

    ~UVPoolHelper()
    {
    }

    inline void *Malloc(size_t size) { return _mempool.AllocBlock(size); }
    inline void Free(void *p) { _mempool.FreeBlock(p); }

    template <typename T>
    T *Construct(T * = NULL) { return GetPool<T>((T *)NULL).Construct(); }
    template <typename T>
    void Destroy(T *p) { GetPool<T>((T *)NULL).Destroy(p); }

    template <typename T>
    ObjectPool<T> &GetPool(T *);

    template <typename T>
    ObjectPool<UVData> &GetPool(UVData * = NULL) { return _data; }
    template <typename T>
    ObjectPool<UVReqWrite> &GetPool(UVReqWrite * = NULL) { return _reqwrite; }
    template <typename T>
    ObjectPool<uv_connect_t> &GetPool(uv_connect_t * = NULL) { return _uvconnect; }
    template <typename T>
    ObjectPool<uv_write_t> &GetPool(uv_write_t * = NULL) { return _uvwrite; }
    template <typename T>
    ObjectPool<uv_udp_send_t> &GetPool(uv_udp_send_t * = NULL) { return _uvudpsend; }
    template <typename T>
    ObjectPool<uv_tcp_t> &GetPool(uv_tcp_t * = NULL) { return _uvtcp; }
    template <typename T>
    ObjectPool<uv_udp_t> &GetPool(uv_udp_t * = NULL) { return _uvudp; }
    template <typename T>
    ObjectPool<uv_timer_t> &GetPool(uv_timer_t * = NULL) { return _uvtimer; }
    template <typename T>
    ObjectPool<uv_async_t> &GetPool(uv_async_t * = NULL) { return _uvasync; }

protected:
    // Data
    ObjectPool<UVData> _data;

    // Cpp Req
    ObjectPool<UVReqWrite> _reqwrite;

    // uv req
    ObjectPool<uv_connect_t> _uvconnect;
    ObjectPool<uv_write_t> _uvwrite;
    ObjectPool<uv_udp_send_t> _uvudpsend;

    // uv handle
    ObjectPool<uv_tcp_t> _uvtcp;
    ObjectPool<uv_udp_t> _uvudp;
    ObjectPool<uv_timer_t> _uvtimer;
    ObjectPool<uv_async_t> _uvasync;

    // big block allocator
    MemPool<Allocator> _mempool; // 用于大块内存的管理
};

} // namespace XNode

#endif // UVPOOLHELPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
