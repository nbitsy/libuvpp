
#ifndef _BITSET_H_
#define _BITSET_H_

#include "Config.h"
#include <sstream>

namespace XSpace
{

// XXX: idx start from 0

#define POS(x) (1 << ((x)&0x7))

template <s32_t SIZE = 8>
class BitSet
{
public:
    BitSet() : _byteCount(0)
    {
        Reset();
    }

    ~BitSet() {}

    // 7 <---- 0
    // 15<---- 8

    inline bool Test(u32_t bit) const { return Get(bit); }

    inline bool Get(u32_t bit) const
    {
        u32_t idx = bit >> 3;
        if (idx >= ByteUsed())
            return false;
        return _bits[bit >> 3] & POS(bit);
    }

    inline void Set(u32_t bit, bool val)
    {
        if (bit >= SIZE)
            return;

        u32_t idx = bit >> 3;
        if (val)
            _bits[idx] |= POS(bit);
        else
            _bits[idx] &= ~POS(bit);

        if (_byteCount <= idx)
            _byteCount = idx + 1;
    }

    inline void Clear(u32_t bit)
    {
        u32_t idx = bit >> 3;
        if (idx >= ByteUsed())
            return;
        _bits[bit >> 3] &= ~POS(bit);
    }

    inline void Reset() { memset(&_bits, 0x00, sizeof(_bits)); }

    bool Any() const
    {
        for (size_t i = 0; i < sizeof(_bits); ++i)
        {
            if (i != sizeof(_bits) - 1)
            {
                if (_bits[i])
                    return true;
            }
            else
            {
                if (_bits[i] & (0xff >> (8 - (SIZE % 8))))
                    return true;
            }
        }
        return false;
    }

    bool All() const
    {
        for (size_t i = 0; i < sizeof(_bits); ++i)
        {
            if (i != sizeof(_bits) - 1)
            {
                if (_bits[i] != 0xff)
                    return false;
            }
            else
            {
                if (_bits[i] != (0xff >> (8 - (SIZE % 8))))
                    return false;
            }
        }
        return true;
    }

    bool None() const
    {
        for (size_t i = 0; i < sizeof(_bits); ++i)
        {
            if (i != sizeof(_bits) - 1)
            {
                if (_bits[i] != 0x00)
                    return false;
            }
            else
            {
                if (_bits[i] & (0xff >> (8 - (SIZE % 8))))
                    return false;
            }
        }
        return true;
    }

    inline size_t Space() const { return sizeof(_bits); }
    inline size_t ByteUsed() const { return _byteCount; }
    inline size_t Size() const { return sizeof(_bits) * (sizeof(_bits[0])); }
    inline const u8_t *operator&() const { return &_bits[0]; }

    const std::string ToString() const
    {
        std::stringbuf sb;
        for (int i = 0; i < sizeof(_bits); ++i)
        {
            u8_t v = _bits[i];
            for (int j = 0; j < 8; ++j)
            {
                if (v & (1 << (7 - j)))
                    sb.sputc('1');
                else
                    sb.sputc('0');
            }
        }
        return sb.str();
    }

protected:
    u32_t _byteCount;
    u8_t _bits[(SIZE + 7) >> 3];
};

#undef POS

} // namespace XSpace

#endif // _BITSET_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
