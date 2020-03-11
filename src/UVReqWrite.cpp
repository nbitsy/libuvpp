
#include "UVReqWrite.h"
#include "UVIODevice.h"
#include "UVLoop.h"

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
            UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)req->handle);
            if (uvdata != NULL && uvdata->_self != NULL)
                ((UVIODevice *)uvdata->_self)->Close();
        }
    }

    // OnReq必须在最后面被调用
    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata)
        return;

    UVReqWrite* uvreqwrite = (UVReqWrite*)uvdata->_self;
    if (NULL == uvreqwrite)
        return;

    uvreqwrite->OnReq(status);
    uvreqwrite->Release();
}

static void __OnWriteUDP(uv_udp_send_t *req, int status)
{
    if (NULL == req)
        return;

    if (status < 0)
    {
        if (req->handle != NULL)
        {
            UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)req->handle);
            if (uvdata != NULL && uvdata->_self != NULL)
                ((UVIODevice *)uvdata->_self)->Close();
        }
    }

    // OnReq必须在最后面被调用
    UVData *uvdata = (UVData *)uv_req_get_data((uv_req_t *)req);
    if (NULL == uvdata)
        return;

    UVReqWrite* uvreqwrite = (UVReqWrite*)uvdata->_self;
    if (NULL == uvreqwrite)
        return;

    uvreqwrite->OnReq(status);
    uvreqwrite->Release();
}

void UVReqWrite::Init(UVIODevice *uviodevice, void *data, int nsize, bool copy)
{
    d._data = data;
    d._nsize = nsize;

    if (copy && data != NULL && nsize > 0)
    {
        d._data = Allocator::malloc(nsize);
        if (d._data != NULL)
        {
            _bCopye = true;
            memcpy(d._data, data, nsize);
        }
    }

    InitReq(uviodevice);
}

void UVReqWrite::InitReq(UVIODevice *uviodevice)
{
    auto loop = GetLoop();
    if (NULL == loop)
    {
        std::cerr << "GetLoop get a null value!!!" << std::endl;
        return;
    }

    uv_handle_t* handle = uviodevice->GetHandle<uv_handle_t>();
    if (NULL == handle)
        return;

    if (handle->type == UV_TCP || handle->type == UV_TTY)
    {
        _req = (uv_req_t *)loop->Construct<uv_write_t>();
    }
    else if (handle->type == UV_UDP)
    {
        _req = (uv_req_t *)loop->Construct<uv_udp_send_t>();
    }
    else
    {
        ERROR("^^^^^^^^^^^^^Unkonw handle!!!^^^^^^^^^^^^^\n");
    }

    if (_req != NULL)
    {
        uv_req_set_data(_req, NULL);
        SetData(NULL);
    }
}

void UVReqWrite::Init(UVIODevice *uviodevice, void *bufs[], int nbuf, bool copy)
{
    d2._bufs = bufs;
    d2._nbuf = nbuf;

    if (copy && bufs != NULL && nbuf > 0)
    {
        d2._bufs = (void **)Allocator::malloc(nbuf * sizeof(void *));
        if (d2._bufs != NULL)
        {
            _bCopye = true;
            for (int i = 0; i < nbuf; ++i)
            {
                int size = *((int *)bufs[i]);
                void *buf = (void *)Allocator::malloc(size + sizeof(size));

                if (buf)
                    memcpy(buf, bufs[i], size + sizeof(size));

                d2._bufs[i] = buf;
            }
        }
    }

    InitReq(uviodevice);
}

UVReqWrite::UVReqWrite(UVIODevice *uviodevice, UVIODevice *other, void *data, int nsize, bool copy, bool gc)
    : UVReq(gc), _bCopye(copy), _uviodevice(uviodevice), _other(other), _bBuffers(false)
{
    Init(uviodevice, data, nsize, copy);
}

UVReqWrite::UVReqWrite(UVIODevice *uviodevice, UVIODevice *other, void *bufs[], int nbuf, bool copy, bool gc)
    : UVReq(gc), _bCopye(false), _uviodevice(uviodevice), _other(other), _bBuffers(true)
{
    Init(uviodevice, bufs, nbuf, copy);
}

UVReqWrite::UVReqWrite(UVIODevice *uviodevice, const struct sockaddr *addr, void *data, int nsize, bool copy, bool gc)
    : UVReq(gc), _bCopye(false), _uviodevice(uviodevice), _bBuffers(false), _other(NULL), _addr(NULL)
{
    Init(uviodevice, data, nsize, copy);
    InitAddress(addr);
}

UVReqWrite::UVReqWrite(UVIODevice *uviodevice, const struct sockaddr *addr, void *bufs[], int nbuf, bool copy, bool gc)
    : UVReq(gc), _bCopye(false), _uviodevice(uviodevice), _bBuffers(true), _other(NULL), _addr(NULL)
{
    Init(uviodevice, bufs, nbuf, copy);
    InitAddress(addr);
}

void UVReqWrite::InitAddress(const struct sockaddr *addr)
{
    if (addr != NULL)
    {
        if (addr->sa_family == AF_INET6)
            _addr = (struct sockaddr *)Allocator::malloc(sizeof(struct sockaddr_in6));
        else // INET4
            _addr = (struct sockaddr *)Allocator::malloc(sizeof(struct sockaddr_in));

        if (_addr != NULL)
            *_addr = *addr;
    }
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
                free(d2._bufs[i]); // TODO:

            free(d2._bufs); // TODO:
        }

        d2._bufs = NULL;
        d2._nbuf = 0;
    }

    if (_addr != NULL)
    {
        free(_addr); // TODO:
        _addr = NULL;
    }
}

bool UVReqWrite::Start()
{
    if (NULL == _req || NULL == _uviodevice)
        return false; // TODO: 回收自己
    uv_handle_t *uvhandle = _uviodevice->GetHandle<uv_handle_t>();
    if (NULL == uvhandle)
        return false;

    uv_buf_t *uvbuf = NULL;
    int nbufs = 0;
    uv_buf_t buf;

    if (!_bBuffers)
    {
        buf = uv_buf_init((char *)d._data, d._nsize);
        uvbuf = &buf;
        nbufs = 1;
    }
    else
    {
        if (d2._nbuf <= 0 || NULL == d2._bufs)
            return false;

        uv_buf_t *bufs = (uv_buf_t *)Allocator::malloc(sizeof(uv_buf_t) * d2._nbuf);
        if (NULL == bufs)
            return false;

        for (int i = 0; i < d2._nbuf; ++i)
        {
            void *data = d2._bufs[i];
            int size = *(int *)data;
            bufs[i] = uv_buf_init((char *)((char *)data + sizeof(size)), size);
        }

        uvbuf = bufs;
        nbufs = d2._nbuf;
    }

    if (NULL == uvbuf)
        return false; // TODO: 回收自己

    int type = uvhandle->type;
    if (type == UV_TCP || type == UV_TTY)
    {
        if (NULL == _other)
        {
            if (!uv_write(GetReq<uv_write_t>(), (uv_stream_t *)uvhandle, uvbuf, nbufs, __OnWrite))
                return true;
        }
        else
        {
            if (!uv_write2(GetReq<uv_write_t>(), (uv_stream_t *)uvhandle, uvbuf, nbufs, _other->GetHandle<uv_stream_t>(), __OnWrite))
                return true;
        }
    }
    else if (type == UV_UDP)
    {
        if (NULL == _addr)
            return false;
        if (!uv_udp_send(GetReq<uv_udp_send_t>(), (uv_udp_t *)uvhandle, uvbuf, nbufs, _addr, __OnWriteUDP))
            return true;
    }

    return false; // TODO: 回收自己
}

UVLoop *UVReqWrite::GetLoop()
{
    if (NULL == _uviodevice)
        return NULL;

    return _uviodevice->GetLoop();
}

void UVReqWrite::OnReq(int status)
{
    DEBUG("\n");
}

void UVReqWrite::Release()
{
    // XXX: 这里释放的顺序不能乱
    if (NULL ==_req)
        return;

    auto loop = GetLoop();
    if (NULL == loop)
    {
        DEBUG("********************loop is null!!!********************\n");
        return;
    }

    ClearData();
    loop->Destroy((uv_write_t*)_req);
    if (GetGC())
        loop->Destroy(this);

    _req = NULL;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
