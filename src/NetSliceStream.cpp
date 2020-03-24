
#include "NetSliceStream.h"
#include "Allocator.h"
#include "Slice.h"

namespace XSpace
{

NetSliceStream::NetSliceStream(UVLoop *loop, int flags)
    : UVTcp(loop, flags), _readBroken(false), _readBrokenBuffer(0)
{
    DEBUG("\n");
}

NetSliceStream::~NetSliceStream()
{
    DEBUG("\n");
}

UVStream *NetSliceStream::OnNewConnection()
{
    return Allocator::Construct<NetSliceStream>(GetLoop());
}

void NetSliceStream::OnAccepted(UVStream *server)
{
    DEBUG("\n");
}

void NetSliceStream::OnConnected()
{
    DEBUG("\n");
}

void NetSliceStream::PushSlice(void *data, int nsize, unsigned int flags)
{
    DEBUG("LEN(flags:%x): %d => %s\n", flags, nsize, (char *)data);
}

void NetSliceStream::ClearReadBrokenBuffer()
{
    DEBUG("\n");
    if (_readBrokenBuffer != NULL)
    {
        Allocator::free(_readBrokenBuffer);
        _readBrokenBuffer = NULL;
    }

    if (_readBrokenBufferStream != NULL)
    {
        Allocator::Destroy(_readBrokenBufferStream);
        _readBrokenBufferStream = NULL;
    }
}

void NetSliceStream::ClearReadBroken()
{
    ClearReadBrokenBuffer();
    _readBroken = false;
}

MemStream *NetSliceStream::GetSpliceBuffer(int nread)
{
    // 之前有不完整的包，空间够用
    if (_readBrokenBufferStream != NULL && _readBrokenBufferStream->WriteSize() >= nread)
        return _readBrokenBufferStream;

    int alreadyRead = 0;
    if (_readBrokenBuffer != NULL)
        alreadyRead = _readBrokenBufferStream->ReadSize();

    int needspace = nread + alreadyRead;
    int size = READ_BUFFER_SIZE;
    while (size < needspace)
        size <<= 1;

    if (size > READ_BUFFER_SIZE_MAX)
    {
        // 包太大了，全部丢弃，包括缓存里的
        ClearReadBroken();
        return NULL;
    }

    // 之前有不完整的包，空间不够用，重新分配足够的空间
    if (_readBrokenBuffer != NULL)
    {
        void *readBrokenBuf = Allocator::malloc(size);
        if (NULL == readBrokenBuf)
        {
            ClearReadBroken();
            return NULL;
        }

        memcpy(readBrokenBuf, _readBrokenBufferStream->C_ReadPos(), alreadyRead);
        MemStream *readBufStream = Allocator::Construct<MemStream>((unsigned char *)readBrokenBuf, size, alreadyRead);
        if (NULL == readBufStream)
        {
            Allocator::free(readBrokenBuf);
            ClearReadBroken();
            return NULL;
        }

        ClearReadBrokenBuffer();

        _readBrokenBuffer = readBrokenBuf;
        _readBrokenBufferStream = readBufStream;
        return _readBrokenBufferStream;
    }
    else
    {
        // 之前没有不完整包
        _readBrokenBuffer = Allocator::malloc(size);
        if (NULL == _readBrokenBuffer)
        {
            ClearReadBroken();
            return NULL;
        }

        _readBrokenBufferStream = Allocator::Construct<MemStream>((unsigned char *)_readBrokenBuffer, size);
        if (NULL == _readBrokenBufferStream)
        {
            ClearReadBroken();
            return NULL;
        }

        return _readBrokenBufferStream;
    }

    return NULL;
}

bool NetSliceStream::HasSpliceSlice() const
{
    if (_readBrokenBuffer != NULL)
    {
        Slice *slice = (Slice *)_readBrokenBufferStream->C_ReadPos();
        if (slice->Length > 0 && slice->Length <= _readBrokenBufferStream->ReadSize())
            return true;
    }

    return false;
}

void NetSliceStream::OnRead(void *data, int nread)
{
#if _DEBUG
    // TODO: for test
    Write((void *)"1", 1);
#endif

    DEBUG("\n");
    MemStream ms((unsigned char *)data, nread, nread);
    MemStream *pms = &ms;
    while (pms != NULL && pms->ReadSize())
    {
        // 一个新包开始
        if (!_readBroken)
        {
            // 解出收到的数据包里所有的完整包
            while (pms->ReadSize())
            {
                Slice *slice = (Slice *)pms->C_ReadPos();
                // 有包头不完整的包
                if (pms->ReadSize() < slice->HeadLength())
                    break;

                // 有不完整包休的包
                if (slice->Length > pms->ReadSize())
                    break;

                PushSlice(slice->Body(), slice->BodyLength(), slice->Flags);
                pms->ReadFlipSilence(slice->Length);
            }

            // 有不完整包
            if (pms->ReadSize())
                _readBroken = true;
        }

        if (_readBroken)
        {
            // 缓存起不完整的包，并与之前不完整包合并
            pms = GetSpliceBuffer(pms->ReadSize());
            if (pms != NULL && _readBroken) // GetSpliceBuffer里可能会打断拼包，而这个打断会把所有待拼数据抛弃
            {
                pms->Append(data, nread);
                if (HasSpliceSlice())
                {
                    _readBroken = false;
                    continue;
                }
            }
            else if (!_readBroken)
            {
                ERROR("Mmeory not enought! Give up all data are not complete!\n");
            }
        }

        if (!_readBroken)
            ClearReadBrokenBuffer();

        break;
    }
}

void NetSliceStream::OnClosed()
{
    DEBUG("\n");
}

void NetSliceStream::OnShutdown()
{
    DEBUG("\n");
}

void NetSliceStream::Release()
{
    DEBUG("\n");
    ClearReadBroken();
    UVTcp::Release();
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
