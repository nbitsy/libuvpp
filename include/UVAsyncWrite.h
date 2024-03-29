
#ifndef _UVASYNCWRITE_H_
#define _UVASYNCWRITE_H_

#include <deque>

#include "SyncDeque.h"
#include "UVAsync.h"
#include "Slice.h"

namespace XSpace
{

class UVIODevice;

struct UVAsyncWriteData
{
    UVAsyncWriteData(void* data, int length, bool copy = true, ESliceType slice = EST_NONE)
        : Data(NULL), Length(length), MsgID(0), SliceType(slice)
    {
        DEBUG("Object @%p\n", this);
        Construct(data, length, copy);
    }

    UVAsyncWriteData(void* data, int length, unsigned int msgid, bool copy = true, ESliceType slice = EST_NONE)
        : Data(NULL), Length(length), MsgID(msgid), SliceType(slice)
    {
        DEBUG("Object @%p\n", this);
        Construct(data, length, copy);
    }

    void Construct(void* data, int length, bool copy = true)
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

    void* Data;
    int Length;
    unsigned int MsgID;
    bool Copyed;
    ESliceType SliceType;
};

class UVAsyncWrite : public UVAsync
{
public:
    UV_CREATE_HANDLE(UVAsyncWrite)

public:
    UVAsyncWrite(const std::weak_ptr<UVLoop>& loop, const std::weak_ptr<UVIODevice>& iodevice);
    ~UVAsyncWrite();

    void OnAsync() OVERRIDE;

    void Send(void* data, int nwrite, bool copy = true, ESliceType slice = EST_SLICE);
    void Send(void* data, int nwrite, unsigned int msgid, bool copy = true, ESliceType slice = EST_NONE);

protected:
    void Append(void* data) OVERRIDE;

private:
    // 一个异步写对象对应一个Handle
    std::weak_ptr<UVIODevice> _iodevice; // 对宿主的弱引用，宿主对我是强引用
    SyncDeque<UVAsyncWriteData*> _queue;
};

} // namespace XSpace

#endif // _UVASYNCWRITE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
