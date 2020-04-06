
#ifndef _UVASYNCWRITE_H_
#define _UVASYNCWRITE_H_

#include <deque>

#include "SyncDeque.h"
#include "UVAsync.h"

namespace XSpace
{

class UVIODevice;

struct UVAsyncWriteData
{
    UVAsyncWriteData(void *data, int length, bool copy = true) : Data(NULL), Length(length)
    {
        DEBUG("Object @%p\n", this);
        if (copy)
        {
            Data = Allocator::malloc(length);
            if (Data != NULL)
                std::memcpy(Data, data, length);
            else
                ERROR("Malloc!!!\n");
        }
        else
        {
            Data = data;
            Length = length;
        }

        Copyed = copy;
    }

    ~UVAsyncWriteData()
    {
        DEBUG("Object @%p\n", this);
        if (Copyed && Data != NULL)
            Allocator::free(Data);
    }

    void *Data;
    int Length;
    bool Copyed;
};

class UVAsyncWrite : public UVAsync
{
public:
    UV_CREATE_HANDLE(UVAsyncWrite)

public:
    ~UVAsyncWrite();

    void OnAsync() OVERRIDE;
    void Append(void *data) OVERRIDE;

    void Send(void *data, int nwrite, bool copy = true);

protected:
    UVAsyncWrite(const std::weak_ptr<UVLoop> &loop, const std::weak_ptr<UVIODevice> &iodevice, bool sendSlice = false);

private:
    // 一个异步写对象对应一个Handle
    std::weak_ptr<UVIODevice> _iodevice; // 对宿主的弱引用，宿主对我是强引用
    SyncDeque<UVAsyncWriteData *> _queue;
    bool _sendSlice; // 发送的数据包是否加上Slice的包头
};

} // namespace XSpace

#endif // _UVASYNCWRITE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
