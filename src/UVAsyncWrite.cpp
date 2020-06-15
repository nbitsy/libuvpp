

#include "UVAsyncWrite.h"
#include "NetSliceStream.h"
#include "UVIODevice.h"
#include "UVLoop.h"

namespace XSpace
{

UVAsyncWrite::UVAsyncWrite(const std::weak_ptr<UVLoop>& loop, const std::weak_ptr<UVIODevice>& iodevice, bool sendSlice)
    : UVAsync(loop), _iodevice(iodevice), _makeSliceBeforeSend(sendSlice)
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

    SyncDeque<UVAsyncWriteData*> tmp;
    _queue.Swap(tmp);

    auto& iodevice = _iodevice;
    auto& makeSliceBeforeSend = _makeSliceBeforeSend;

    tmp.ForEach([iodevice, makeSliceBeforeSend](UVAsyncWriteData* data) {
        if (!iodevice.expired())
        {
            auto device = iodevice.lock().get();
            DEBUG("Writing data @%p length: %d\n", data->Data, data->Length);
            if (makeSliceBeforeSend)
                ((NetSliceStream*)device)->Write(data->Data, data->Length, data->MsgID);
            else
                device->Write(data->Data, data->Length);
        }
        Allocator::Destroy(data);
    });
}

void UVAsyncWrite::Append(void* data)
{
    if (NULL == data)
        return;

    _queue.PushBack((UVAsyncWriteData*)data);
}

void UVAsyncWrite::Send(void* data, int nwrite, bool copy, bool dataisslice)
{
    UVAsyncWriteData* uvasyncwritedata = Allocator::Construct<UVAsyncWriteData>(data, nwrite, copy);
    if (NULL == uvasyncwritedata)
        return;

    // 如果data里存放的本身就是一个slice，则不需要拼装Slice了
    // XXX: 默认情况是需要拼装的
    _makeSliceBeforeSend = !dataisslice;
    UVAsync::Send(uvasyncwritedata);
}

void UVAsyncWrite::Send(void* data, int nwrite, unsigned int msgid, bool copy, bool dataisslice)
{
    UVAsyncWriteData* uvasyncwritedata = Allocator::Construct<UVAsyncWriteData>(data, nwrite, msgid, copy);
    if (NULL == uvasyncwritedata)
        return;

    _makeSliceBeforeSend = !dataisslice;
    UVAsync::Send(uvasyncwritedata);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
