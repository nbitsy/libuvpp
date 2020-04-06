

#include "UVAsyncWrite.h"
#include "UVIODevice.h"
#include "UVLoop.h"
#include "NetSliceStream.h"

namespace XSpace
{

UVAsyncWrite::UVAsyncWrite(const std::weak_ptr<UVLoop> &loop, const std::weak_ptr<UVIODevice> &iodevice, bool sendSlice)
    : UVAsync(loop), _iodevice(iodevice), _sendSlice(sendSlice)
{
    DEBUG("Object @%p\n", this);
    DEBUG("Register to loop: %p\n", _loop.lock()->FormatedThreadId());
}

UVAsyncWrite::~UVAsyncWrite()
{
    DEBUG("Object @%p\n", this);
}

void UVAsyncWrite::OnAsync()
{
    DEBUG("Writing data ... \n");
    if (_iodevice.expired())
        return;

    SyncDeque<UVAsyncWriteData *> tmp;
    _queue.Swap(tmp);

    auto &iodevice = _iodevice;
    auto &sendSlice = _sendSlice;
    tmp.ForEach([&iodevice, &sendSlice](UVAsyncWriteData *data) {
        if (!iodevice.expired())
        {
            auto device = iodevice.lock().get();
            INFO("Writing data @%p length: %d\n", data->Data, data->Length);
            if (sendSlice)
                ((NetSliceStream*)device)->Write(data->Data, data->Length);
            else
                device->Write(data->Data, data->Length);
        }
        Allocator::Destroy(data);
    });
}

void UVAsyncWrite::Append(void *data)
{
    if (NULL == data)
        return;

    _queue.PushBack((UVAsyncWriteData *)data);
}

void UVAsyncWrite::Send(void *data, int nwrite, bool copy)
{
    UVAsyncWriteData *uvasyncwritedata = Allocator::Construct<UVAsyncWriteData>(data, nwrite, copy);
    if (NULL == uvasyncwritedata)
        return;
    
    UVAsync::Send(uvasyncwritedata);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
