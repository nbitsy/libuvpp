
#include "UVStream.h"
#include "UVLoop.h"
#include "UVReqShutdown.h"
#include "UVReqWrite.h"
#include <iostream>

namespace XSpace
{

static void __OnNewConnection(uv_stream_t *server, int status)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)server);
    if (NULL == uvdata)
        return;

    UVStream *uvstream = uvdata->GetPtr<UVStream>();
    if (NULL == uvstream)
        return;

    std::shared_ptr<UVHandle> newuvstream = uvstream->OnNewConnection();
    if (NULL == newuvstream)
    {
        std::cerr << "Create connection error!!!" << std::endl;
        return;
    }

    std::weak_ptr<UVHandle> stream(newuvstream);
    uvstream->Accept(stream);
}

UVStream::UVStream(std::weak_ptr<UVLoop> &loop, int flags, EUVStreamType type)
    : UVIODevice(loop, flags), _type(type)
{
    DEBUG("Object @%p\n", this);
}

UVStream::~UVStream()
{
    DEBUG("Object @%p\n", this);
}

bool UVStream::Listen(int backlog)
{
    if (NULL == _handle)
        return false;

    int r = uv_listen(GetHandle<uv_stream_t>(), backlog, __OnNewConnection);
    if (r)
    {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return false;
    }

    return true;
}

bool UVStream::Accept(std::weak_ptr<UVHandle> &client)
{
    if (NULL == _handle || client.expired())
        return false;

    auto c = client.lock();
    if (c->GetHandle<uv_handle_t>() == NULL)
        return false;

    if (uv_accept(GetHandle<uv_stream_t>(), c->GetHandle<uv_stream_t>()))
        return false;

    UVStream *stream = (UVStream *)c.get();
    if (_type & EUVS_READ)
        stream->StartRead();

    OnAccept(client);
    InitAddress();
    std::weak_ptr<UVHandle> ss(shared_from_this());
    stream->OnAccepted(ss);
    return true;
}

bool UVStream::IsReadable() const
{
    if (NULL == _handle)
        return false;

    return uv_is_readable(GetHandle<uv_stream_t>());
}

bool UVStream::IsWritable() const
{
    if (NULL == _handle)
        return false;

    return uv_is_writable(GetHandle<uv_stream_t>());
}

size_t UVStream::GetWriteQueueSize() const
{
    if (NULL == _handle)
        return 0;

    return uv_stream_get_write_queue_size(GetHandle<uv_stream_t>());
}

bool UVStream::Shutdown()
{
    if (NULL == _handle)
        return false;

    std::weak_ptr<UVHandle> self(shared_from_this());
    std::shared_ptr<UVReqShutdown> req = UVReqShutdown::Create<UVReqShutdown>(self);
    if (req != NULL)
        return req->Start();

    return false;
}

void UVStream::SetBlocking(bool v)
{
    if (NULL == _handle)
        return;

    uv_stream_set_blocking(GetHandle<uv_stream_t>(), v);
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
