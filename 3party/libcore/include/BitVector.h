
#ifndef _BITVEC_H_
#define _BITVEC_H_

#include "Config.h"
#include <string>
#include <vector>

namespace XSpace
{

// XXX: idx start from 0

#define POS(x) (1 << ((x)&0x7))

class BitVector
{
public:
    typedef std::vector<unsigned char> BitsConatiner;

public:
    BitVector() : _size(32 * 8), _byteCount(0), _bits(32, 0) {}
    explicit BitVector(u32_t bits)
        : _size(bits), _byteCount(0), _bits((bits + 7) >> 3, 0) {}
    ~BitVector() {}

    // 7 <---- 0
    // 15<---- 8

    inline bool Test(u32_t bit) { return Get(bit); }

    inline bool Get(u32_t bit)
    {
        u32_t idx = bit >> 3;
        if (idx >= ByteUsed())
            return false;
        return _bits[idx] & POS(bit);
    }

    void Set(u32_t bit, bool val = true);
    inline bool operator[](u32_t bit) { return Get(bit); }

    inline void Clear(u32_t bit)
    {
        u32_t idx = bit >> 3;
        if (idx >= ByteUsed())
            return;
        _bits[bit >> 3] &= ~POS(bit);
    }

    inline void Reset() { memset(&_bits[0], 0x00, ByteUsed()); }
    inline size_t Space() const { return _bits.size(); }
    inline size_t ByteUsed() const { return _byteCount; }
    inline const u8_t *operator&() const { return &_bits[0]; }

    inline u32_t Size() const { return _size; }

    bool Any() const;
    bool All() const;
    bool None() const;

    const std::string ToString() const;

private:
    bool Checksize(u32_t bit)
    {
        if (bit >= _size)
            _size = bit + 1;
        if (((bit + 7) >> 3) >= static_cast<u32_t>(_bits.size()))
            _bits.resize(((bit + 7) >> 3) + 32, 0);
        return true;
    }

protected:
    u32_t _size;
    u32_t _byteCount;
    BitsConatiner _bits;
};

} // namespace XSpace

#endif // _BITVEC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
