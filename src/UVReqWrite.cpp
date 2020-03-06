
#include "UVReqWrite.h"
#include "UVStream.h"

#include <stdlib.h> // TODO: for malloc

namespace XNode
{

static void __OnWrite(uv_write_t *req, int status)
{
    if (NULL == req)
        return;

    if (status < 0)
    {
        if (req->handle != NULL)
        {
            UVData *sdata = (UVData *)uv_handle_get_data((uv_handle_t *)req->handle);
            if (sdata != NULL && sdata->_self != NULL)
                ((UVStream *)sdata->_self)->Close();
        }
    }

    // OnReq必须在最后面被调用
    UVData *data = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (data != NULL && data->_self != NULL)
        ((UVReqWrite *)data->_self)->OnReq(status);
}

UVReqWrite::UVReqWrite(UVStream *stream, UVStream *other, void *data, int nsize, bool copy, bool gc)
    : UVReq(gc), _bCopye(false), _stream(stream), _other(other), _bBuffers(false)
{
    d._data = data;
    d._nsize = nsize;

    if (copy && data != NULL && nsize > 0)
    {
        d._data = malloc(nsize); // TODO: new
        if (d._data != NULL)
        {
            _bCopye = true;
            memcpy(d._data, data, nsize);
        }
    }

    _req = (uv_req_t *)malloc(sizeof(uv_write_t)); // TODO:
    if (_req != NULL)
        SetData(NULL);
}

UVReqWrite::UVReqWrite(UVStream *stream, UVStream *other, void *bufs[], int nbuf, bool copy, bool gc)
    : UVReq(gc), _bCopye(false), _stream(stream), _other(other), _bBuffers(true)
{
    d2._bufs = bufs;
    d2._nbuf = nbuf;

    if (copy && bufs != NULL && nbuf > 0)
    {
        d2._bufs = (void **)malloc(nbuf * sizeof(void *)); // TODO: new
        if (d2._bufs != NULL)
        {
            _bCopye = true;
            for (int i = 0; i < nbuf; ++i)
            {
                int size = *((int *)bufs[i]);
                void *buf = (void *)malloc(size + sizeof(size)); // TODO:

                if (buf)
                    memcpy(buf, bufs[i], size + sizeof(size));

                d2._bufs[i] = buf;
            }
        }
    }

    _req = (uv_req_t *)malloc(sizeof(uv_write_t)); // TODO:
    if (_req != NULL)
        SetData(NULL);
}

UVReqWrite::~UVReqWrite()
{
    if (!_bBuffers)
    {
        if (_bCopye && d._data != NULL)
        {
            free(d._data); // TODO:
            d._data = NULL;
            d._nsize = 0;
        }
    }
    else
    {
        if (_bCopye && d2._bufs && d2._nbuf > 0)
        {
            for (int i = 0; i < d2._nbuf; ++i)
                free(d2._bufs[i]);

            free(d2._bufs);
        }

        d2._bufs = NULL;
        d2._nbuf = 0;
    }

    if (_req != NULL)
    {
        free(_req); // TODO:
        _req = NULL;
    }
}

bool UVReqWrite::Begin()
{
    if (NULL == _req || NULL == _stream)
        return false;

    if (!_bBuffers)
    {
        uv_buf_t buf = uv_buf_init((char *)d._data, d._nsize);

        if (NULL == _other)
        {
            if (uv_write(GetReq<uv_write_t>(), _stream->GetHandle<uv_stream_t>(), &buf, 1, __OnWrite))
                return false;
        }
        else
        {
            if (uv_write2(GetReq<uv_write_t>(), _stream->GetHandle<uv_stream_t>(), &buf, 1, _other->GetHandle<uv_stream_t>(), __OnWrite))
                return false;
        }
        return true;
    }
    else
    {
        if (d2._nbuf <= 0 || NULL == d2._bufs)
            return false;

        uv_buf_t *buf = (uv_buf_t *)malloc(sizeof(uv_buf_t) * d2._nbuf);
        if (NULL == buf)
            return false;

        for (int i = 0; i < d2._nbuf; ++i)
        {
            void *data = d2._bufs[i];
            int size = *(int *)data;
            buf[i] = uv_buf_init((char *)((char *)data + sizeof(size)), size);
        }

        if (NULL == _other)
        {
            if (uv_write(GetReq<uv_write_t>(), _stream->GetHandle<uv_stream_t>(), buf, d2._nbuf, __OnWrite))
                return false;
        }
        else
        {
            if (uv_write2(GetReq<uv_write_t>(), _stream->GetHandle<uv_stream_t>(), buf, d2._nbuf, _other->GetHandle<uv_stream_t>(), __OnWrite))
                return false;
        }

        return true;
    }

    return false;
}

void UVReqWrite::OnReq(int status)
{
    if (GetGC())
        delete this; // TODO: delete or pool release
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
