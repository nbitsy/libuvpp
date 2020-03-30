
#ifndef _UVREQWRITE_H_
#define _UVREQWRITE_H_

#include "TypeTraits.h"
#include "UVReq.h"

namespace XSpace
{

class UVHandle;

class UVReqWrite : public UVReq
{
public:
    template <typename T>
    static std::shared_ptr<UVReqWrite> Create(const std::weak_ptr<UVHandle> &iohandle, const std::weak_ptr<UVHandle> &other, void *data, int nsize, bool copy = false)
    {
        if (is_subclass<T, UVReqWrite>::value)
        {
            std::shared_ptr<UVReqWrite> ptr(new T(iohandle, other, data, nsize, copy));
            if (ptr != NULL)
                ptr->SetData(NULL, true, true);

            return ptr;
        }

        return NULL;
    }
    template <typename T>
    static std::shared_ptr<UVReqWrite> Create(const std::weak_ptr<UVHandle> &iohandle, const std::weak_ptr<UVHandle> &other, void *bufs[], int nbuf, bool copy = false)
    {
        if (is_subclass<T, UVReqWrite>::value)
        {
            std::shared_ptr<UVReqWrite> ptr(new T(iohandle, other, bufs, nbuf, copy));
            if (ptr != NULL)
                ptr->SetData(NULL, true, true);

            return ptr;
        }

        return NULL;
    }
    template <typename T>
    static std::shared_ptr<UVReqWrite> Create(const std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *data, int nsize, bool copy = false, bool gc = true)
    {
        if (is_subclass<T, UVReqWrite>::value)
        {
            std::shared_ptr<UVReqWrite> ptr(new T(iohandle, addr, data, nsize, copy));
            if (ptr != NULL)
                ptr->SetData(NULL, true, true);

            return ptr;
        }

        return NULL;
    }
    template <typename T>
    static std::shared_ptr<UVReqWrite> Create(const std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *bufs[], int nbuf, bool copy = false)
    {
        if (is_subclass<T, UVReqWrite>::value)
        {
            std::shared_ptr<UVReqWrite> ptr(new T(iohandle, bufs, nbuf, copy));
            if (ptr != NULL)
                ptr->SetData(NULL, true, true);

            return ptr;
        }

        return NULL;
    }

private:
    /**
     * UVHandle 输出目标流对象
     * other 管道目标流对象
     * data 数据
     * nsize 数据长
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(const std::weak_ptr<UVHandle> &iohandle, const std::weak_ptr<UVHandle> &other, void *data, int nsize, bool copy = false);

    /**
     * UVHandle 输出目标流对象
     * other 管道目标流对象
     * bufs 数据缓存对象数组，每个数据包的数据长度在头4个字节中，就是 *(int*)bufs[i] 为这个包的长度，不包括sizeof(int)
     * nbuf 缓存区中数据包个数
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(const std::weak_ptr<UVHandle> &iohandle, const std::weak_ptr<UVHandle> &other, void *bufs[], int nbuf, bool copy = false);

    /**
     * UVHandle 输出目标流对象
     * addr UDP时的目标地址
     * data 数据
     * nsize 数据长
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(const std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *data, int nsize, bool copy = false);
    /**
     * UVHandle 输出目标流对象
     * addr UDP时的目标地址
     * bufs 数据缓存对象数组，每个数据包的数据长度在头4个字节中，就是 *(int*)bufs[i] 为这个包的长度，不包括sizeof(int)
     * nbuf 缓存区中数据包个数
     * copy 是否把数据复制到Req对象
     * gc 完成任务后是否回收Req对象本身
    */
    UVReqWrite(const std::weak_ptr<UVHandle> &iohandle, const struct sockaddr *addr, void *bufs[], int nbuf, bool copy = false);

public:
    ~UVReqWrite();

    bool Start() OVERRIDE;
    void OnReq(int status) OVERRIDE;

private:
    void Init(const std::weak_ptr<UVHandle> &iohandle, void *data, int nsize, bool copy);
    void Init(const std::weak_ptr<UVHandle> &iohandle, void *bufs[], int nbuf, bool copy);
    void InitReq(const std::weak_ptr<UVHandle> &iohandle);
    void InitAddress(const struct sockaddr *addr);

private:
    // 是否复制源数据
    bool _bCopye;
    std::weak_ptr<UVHandle> _iohandle;
    std::weak_ptr<UVHandle> _other;
    struct sockaddr *_addr;

    bool _bBuffers;
    union {
        struct
        {
            void *_data;
            int _nsize;
        } d;

        struct
        {
            void **_bufs;
            int _nbuf;
        } d2;
    };
};

} // namespace XSpace

#endif // _UVREQWRITE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
