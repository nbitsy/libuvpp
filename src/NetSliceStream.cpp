
#include "NetSliceStream.h"
#include "Allocator.h"

namespace XSpace
{

NetSliceStream::NetSliceStream(const std::weak_ptr<UVLoop>& loop, int flags)
    : UVTcp(loop, flags), _readBroken(false),
      _readBrokenBuffer(0), _readBrokenBufferStream(0),
      _writeSlice(0), _writeSliceLength(0)
{
    DEBUG("Object @%p\n", this);
}

NetSliceStream::~NetSliceStream()
{
    DEBUG("Object @%p\n", this);
}

std::shared_ptr<UVHandle> NetSliceStream::OnNewSession()
{
    return NetSliceStream::CreateShared(GetLoop());
}

bool NetSliceStream::RecvedSlice(Slice* slice)
{
    DEBUG("LEN: %d => %s\n", slice->BodyLength(), (char*)slice->Body());
    return true;
}

void NetSliceStream::ClearReadBrokenBuffer()
{
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

MemStream* NetSliceStream::GetSpliceBuffer(int nread)
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
        void* readBrokenBuf = Allocator::malloc(size);
        if (NULL == readBrokenBuf)
        {
            ClearReadBroken();
            return NULL;
        }

        memcpy(readBrokenBuf, _readBrokenBufferStream->C_ReadPos(), alreadyRead);
        MemStream* readBufStream = Allocator::Construct<MemStream>((unsigned char*)readBrokenBuf, size, alreadyRead);
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

        _readBrokenBufferStream = Allocator::Construct<MemStream>((unsigned char*)_readBrokenBuffer, size);
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
        Slice* slice = (Slice*)_readBrokenBufferStream->C_ReadPos();
        if (slice->Length > 0 && slice->Length <= _readBrokenBufferStream->ReadSize())
            return true;
    }

    return false;
}

// 这个slice参数所指向的内容也不允许修改
Slice* NetSliceStream::DealFlags(_NOMODIFY Slice* slice)
{
    DEBUG("LEN(flags:%x): %d => %s\n", slice->Flags, slice->Length, slice->Body());
    // TODO: 需要对应解压和加密选项的处理
    return slice;
}

void NetSliceStream::OnRead(void* data, int nread)
{
    DEBUG("RECV: %d\n", nread);
    MemStream ms((unsigned char*)data, nread, nread);
    MemStream* pms = &ms;
    while (pms != NULL && pms->ReadSize())
    {
        // 一个新包开始
        if (!_readBroken)
        {
            // 解出收到的数据包里所有的完整包
            while (pms->ReadSize())
            {
                Slice* slice = (Slice*)pms->C_ReadPos();
                // 有包头不完整的包
                if (pms->ReadSize() < slice->HeadLength())
                    break;

                // 有不完整包休的包
                if (slice->Length > pms->ReadSize())
                    break;

                Slice* realslice = DealFlags(slice);
                RecvedSlice(realslice);
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

Slice* NetSliceStream::MakeSlice(int nsize, _OUT int& total)
{
    Slice* writeSlice = NULL;
    int size = READ_BUFFER_SIZE;
    total = nsize + sizeof(*writeSlice);

    while (size < total)
        size <<= 1;

    if (size > WRITE_BUFFER_SIZE_MAX)
        return NULL;
        
    void* p = Allocator::malloc(size);
    if (NULL == p)
        return NULL;

    writeSlice = ::new (p) Slice();
    if (NULL == writeSlice)
        return NULL;

    return writeSlice;
}

void NetSliceStream::ReleaseSlice(Slice* slice)
{
    Allocator::free(slice);
}

bool NetSliceStream::Write(void* data, int nsize,
                           unsigned int MsgID, unsigned char FwdTargetType,
                           unsigned int FwdTarget, unsigned int Target)
{
    int total = 0;
    _writeSlice = MakeSlice(nsize, total);
    if (NULL == _writeSlice)
        return false;

    // 总包体长，包括包头
    _writeSlice->Length = nsize + sizeof(*_writeSlice);
    memcpy(_writeSlice->Body(), data, nsize);
    // XXX: flags???

    _writeSlice->MsgID = MsgID;
    _writeSlice->FwdTargetType = FwdTargetType;
    _writeSlice->FwdTarget = FwdTarget;
    _writeSlice->Target = Target;

    bool ret = UVTcp::Write((void*)_writeSlice, total);
    ReleaseSlice(_writeSlice);
    _writeSlice = NULL;
    return ret;
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
