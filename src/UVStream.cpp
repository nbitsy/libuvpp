
#include "UVStream.h"
#include "UVLoop.h"
#include "UVReqShutdown.h"
#include "UVReqWrite.h"

namespace XNode
{

static void __OnNewConnection(uv_stream_t *server, int status)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)server);
    if (uvdata != NULL && uvdata->_self != NULL)
    {
        UVStream *uvstream = (UVStream *)uvdata->_self;
        uvstream->Accept(uvstream->OnNewConnection());
    }
}

UVStream::UVStream(UVLoop *loop, int flags, EUVStreamType type)
    : UVIODevice(loop, flags), _type(type)
{
}

UVStream::~UVStream()
{
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

bool UVStream::Accept(UVStream *client)
{
    if (NULL == _handle || NULL == client || NULL == client->GetHandle<uv_handle_t>())
        return false;

    if (uv_accept(GetHandle<uv_stream_t>(), client->GetHandle<uv_stream_t>()))
        return false;

    if (_type & EUVS_READ)
        client->StartRead();

    OnAccept(client);
    client->OnAccepted(this);
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

    UVReqShutdown *req = new UVReqShutdown(this);
    if (req)
        return req->Start();

    return false;
}

void UVStream::SetBlocking(bool v)
{
    if (NULL == _handle)
        return;

    uv_stream_set_blocking(GetHandle<uv_stream_t>(), v);
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
