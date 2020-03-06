
#include "UVStream.h"
#include "UVReqWrite.h"
#include "UVReqShutdown.h"

namespace XNode
{

static void __OnNewConnection(uv_stream_t *server, int status)
{
    UVData *data = (UVData*)uv_handle_get_data((uv_handle_t*)server);
    if (data != NULL && data->_self != NULL)
    {
        UVStream* uvstream = (UVStream*)data->_self;
        uvstream->Accept(uvstream->OnNewConnection());
    }
}

static void __OnAlloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = (char *)malloc(suggested_size); // TODO: malloc where
    buf->len = suggested_size;
}

static void __OnRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    UVData *data = (UVData *)stream->data;
    if (NULL == data)
        return;

    UVStream *pstream = (UVStream *)data->_self;
    if (NULL == pstream)
        return;

    if (nread <= 0)
    {
        pstream->Close();
        return;
    }

    pstream->OnRead(buf->base, nread);
}

UVStream::UVStream(EUVStreamType type) : _type(type)
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
        client->BeginRead();

    client->OnAccepted(this);
    return true;
}

bool UVStream::BeginRead()
{
    if (NULL == _handle)
        return false;

    int ret = uv_read_start(GetHandle<uv_stream_t>(), __OnAlloc, __OnRead);
    if (ret != 0)
        return false;

    return true;
}

bool UVStream::StopRead()
{
    if (NULL == _handle)
        return false;

    return uv_read_stop(GetHandle<uv_stream_t>()) == 0;
}

bool UVStream::Write2(void *data, int nsize, UVStream *other)
{
    if (NULL == _handle)
        return false;

    UVReqWrite *req = new UVReqWrite(this, other, data, nsize);
    if (req != NULL)
        return req->Begin();

    return false;
}

bool UVStream::Write2(void *bufs[], int nbuf, UVStream *other)
{
    if (NULL == _handle)
        return false;

    UVReqWrite *req = new UVReqWrite(this, other, bufs, nbuf);
    if (req != NULL)
        return req->Begin();

    return false;
}

bool UVStream::TryWrite(void *data, int nsize)
{
    if (NULL == _handle)
        return false;

    uv_buf_t buf = uv_buf_init((char *)data, nsize);
    return uv_try_write(GetHandle<uv_stream_t>(), &buf, 1) == 0;
}

bool UVStream::TryWrite(void *bufs[], int nbuf)
{
    if (NULL == _handle)
        return false;

    if (nbuf <= 0 || NULL == bufs)
        return false;

    uv_buf_t *uvbufs = (uv_buf_t *)malloc(nbuf * sizeof(uv_buf_t));
    if (NULL == uvbufs)
        return false;
    
    for (int i = 0; i < nbuf; ++i)
    {
        void *data = bufs[i];
        int size = *(int *)data;
        uvbufs[i] = uv_buf_init((char *)data + sizeof(size), size);
    }
    return uv_try_write(GetHandle<uv_stream_t>(), uvbufs, nbuf) == 0;
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

    UVReqShutdown* req = new UVReqShutdown(this);
    if (req)
        return req->Begin();

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
