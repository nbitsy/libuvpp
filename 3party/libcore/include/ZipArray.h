
#ifndef _ZIPARRAY_H_
#define _ZIPARRAY_H_

#include <iostream>
#include <math.h>
#include <memory.h>
#include <sstream>
#include <stdlib.h>
#include <unordered_map>

#include "Allocator.h"
#include "Debugger.h"
#include "Serialization.h"
#include "TypeTraits.h"

namespace XSpace
{
/*
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	32	1	256	1024	36	72	7.03125	92.96875	256
2	64	2	256	1024	36	108	10.546875	89.453125	128
3	96	8	256	1024	36	164	16.015625	83.984375	32
4	128	16	256	1024	36	228	22.265625	77.734375	16
5	160	32	256	1024	36	324	31.640625	68.359375	8
6	192	64	256	1024	36	484	47.265625	52.734375	4
7	224	128	256	1024	36	772	75.390625	24.609375	2
8	256	256	256	1024	36	1316	128.515625	-28.515625	1
									
									
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	16	1	128	512	36	56	10.9375	89.0625	128
2	32	2	128	512	36	76	14.84375	85.15625	64
3	48	8	128	512	36	116	22.65625	77.34375	16
4	64	16	128	512	36	164	32.03125	67.96875	8
5	80	32	128	512	36	244	47.65625	52.34375	4
6	96	64	128	512	36	388	75.78125	24.21875	2
7	112	128	128	512	36	660	128.90625	-28.90625	1
									
									
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	8	1	64	256	36	48	18.75	81.25	64
2	16	2	64	256	36	60	23.4375	76.5625	32
3	24	8	64	256	36	92	35.9375	64.0625	8
4	32	16	64	256	36	132	51.5625	48.4375	4
5	40	32	64	256	36	204	79.6875	20.3125	2
6	48	64	64	256	36	340	132.8125	-32.8125	1

结论：当出现的属性条数是最大属性条数的1/8时，可以省一半以上内存，而且会随着最大属性条数越多，省出的内存越多。								
当属性条数越过1/8时，可以转换成全属性存储。 1/4时的收受也是非常可观的。
XXX: 如果存储的数据是8字节收受会更可观。

如果存储的数据是4字节宽的，当数据条数超过1/8时使用全索引空间存储，也就是_value里会分配 _idxmax * sizeof(int) 个字节来存放数据。
否则只申请1/8的存储空间
如果存储的数据是8字节宽的，则申请1/4的 _idxmax * sizeof(long) 的空间来存储数据。当超过1/4时转为全空间存储。
*/

// 至少存储64个值
#define ZIP_ARRAY_SIZE_MIN 64 // XXX: 实际上只能存储63个值，因为第0个索引作为空索引
// 最多256个值，如果超过这个值则转换成hash_map/unordered_map
#define ZIP_ARRAY_SIZE_MAX 256 // XXX: 这个值不允许修改，最多就是256

static const unsigned char ZIPARRAY_ARRAY_NONE = -1;
static const unsigned char ZIPARRAY_ARRAY_1_8 = 0;
static const unsigned char ZIPARRAY_ARRAY_1_4 = 1;
static const unsigned char ZIPARRAY_ARRAY_1_2 = 2;
static const unsigned char ZIPARRAY_FULLARRAY = 3;
static const unsigned char ZIPARRAY_MAP = 4;

typedef unsigned short ZipArrayIdx_t;

// 超过1/8时先切到1/4，超过1/4时切到1/2，超过1/2时切到全量
template <typename T>
class ZipArray : public ISerialization
{
public:
    template <typename U>
    friend class ZipArray;

public:
    typedef std::unordered_map<ZipArrayIdx_t, T> ZipArrayMap_t;

public:
    const static T null;

public:
    explicit ZipArray(ZipArrayIdx_t idxmax = ZIP_ARRAY_SIZE_MIN, unsigned char type = ZIPARRAY_ARRAY_1_8);
    template <typename U>
    ZipArray(ZipArray<U> &other)
        : _count(other._count), _size(other._size), _idxmax(other._idxmax),
          _idxwidth(other._idxwidth), _datatype(other._datatype), _value(NULL), _data(NULL)
    {
        CopyFrom(other);
    }
    template <typename U>
    ZipArray(const ZipArray<U> &other)
        : _count(other._count), _size(other._size), _idxmax(other._idxmax),
          _idxwidth(other._idxwidth), _datatype(other._datatype), _value(NULL), _data(NULL)
    {
        CopyFrom(other);
    }
    ~ZipArray();

    void Clear();

    // 返回引用是一个危险的动作，所以需要明确的调用
    T &GetRef(ZipArrayIdx_t idx) const;
    inline T Get(ZipArrayIdx_t idx) const { return GetRef(idx); }
    inline T operator[](ZipArrayIdx_t idx) const { return GetRef(idx); }

    // 返回原始值
    T Set(ZipArrayIdx_t idx, const T &value);
    // 清空并返回当前值
    T Clear(ZipArrayIdx_t idx);

    inline ZipArrayIdx_t Count() const { return _count; }
    inline ZipArrayIdx_t IdxMax() const { return _idxmax - 1; }
    inline ZipArrayIdx_t Size() const { return _size; }
    inline ZipArrayIdx_t Capacity() const { return _size; }

    inline bool HasData(ZipArrayIdx_t idx) const { return HasData(idx, 0, 0); }

    void Print();
    const std::string ToString() const;

    template <typename U>
    void CopyFrom(const ZipArray<U> &other);

    template <typename U>
    ZipArray<T> &operator=(const ZipArray<U> &other);

    template <typename U>
    ZipArray<T> operator+(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> operator-(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> operator*(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> operator/(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> operator%(const ZipArray<U> &other);

    template <typename U>
    ZipArray<T> &operator+=(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> &operator-=(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> &operator*=(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> &operator/=(const ZipArray<U> &other);
    template <typename U>
    ZipArray<T> &operator%=(const ZipArray<U> &other);

    template <typename U>
    ZipArray<T> &operator+=(const U &v);
    template <typename U>
    ZipArray<T> &operator-=(const U &v);
    template <typename U>
    ZipArray<T> &operator*=(const U &v);
    template <typename U>
    ZipArray<T> &operator/=(const U &v);
    template <typename U>
    ZipArray<T> &operator%=(const U &v);

    virtual void Serialize(OStream &stream) const OVERRIDE;
    virtual void Unserialize(IStream &stream) OVERRIDE;

private:
    bool HasData(ZipArrayIdx_t idx, T **ppvalue, void *map, void *data, unsigned char datatype, unsigned char idxwidth);
    bool HasData(ZipArrayIdx_t idx, T **pvalue, ZipArrayIdx_t *realidx) const;
    void Init(ZipArrayIdx_t idxmax, unsigned char type = ZIPARRAY_ARRAY_1_8);
    void InitFull(ZipArrayIdx_t idxmax);
    void InitHashMap(ZipArrayIdx_t idxmax);
    void CopyDataFromOld(void *src, void *olddata, unsigned char datatype, ZipArrayIdx_t idxmax, unsigned char idxwidth);
    void ConvertTo_1_4();
    void ConvertTo_1_2();
    void ConvertToFull();
    void ConvertToHashMap();

private:
    // 当前元素个数，FULL和MAP之后_count值无效
    ZipArrayIdx_t _count; // _count / _idxmax > 1/8 or _count / _idxmax > 1/4 切到全量
    // 当前空间大小
    ZipArrayIdx_t _size;
    // 索引最大值
    ZipArrayIdx_t _idxmax;
    // 索引位数，FULL和MAP之后_idxwidth值无效
    unsigned char _idxwidth;
    // 当前数据空间类型
    unsigned char _datatype; // 0 - 1/8 1 - 1/4 2 - full 3 - 1/2 4 - hashmap
    // 数据，数据存放空间包括位图数据，位图数据之后是正常数据
    void *_value;
    // 真实数据起始地址
    void *_data;
};

template <typename T>
ZipArray<T>::ZipArray(ZipArrayIdx_t idxmax, unsigned char type)
    : _count(0), _size(0), _idxmax(idxmax),
      _idxwidth(0), _datatype(ZIPARRAY_ARRAY_NONE),
      _value(0), _data(0)
{
    Init(idxmax, type);
}

template <typename T>
ZipArray<T>::~ZipArray()
{
    Clear();
}

template <typename T>
void ZipArray<T>::Clear()
{
    _count = 0;
    _size = 0;
    _idxmax = 0;
    _idxwidth = 0;
    _datatype = ZIPARRAY_ARRAY_NONE;

    if (_datatype < ZIPARRAY_MAP && _value != NULL)
    {
        Allocator::free(_value);
        _value = NULL;
        _data = NULL;
        _datatype = ZIPARRAY_ARRAY_NONE;
    }

    if (ZIPARRAY_MAP == _datatype && _value != NULL)
    {
        Allocator::Destroy((ZipArrayMap_t *)_value);
        _value = NULL;
        _data = NULL;
        _datatype = ZIPARRAY_ARRAY_NONE;
    }
}

inline static unsigned char CalcIdxWidth(int s)
{
    int width = 0;
    while (s > 0)
    {
        s >>= 1;
        ++width;
    }
    return width - 1;
}

template <typename T>
void ZipArray<T>::InitFull(ZipArrayIdx_t idxmax)
{
    DEBUG("\n");
    void *oldvalue = _value;
    void *olddata = _data;
    int oldidxmax = _idxmax;
    int oldidxwidth = _idxwidth;
    int olddatatype = _datatype;

    _value = Allocator::malloc(idxmax * sizeof(T)); // type
    if (NULL == _value)
        return;

    DEBUG("total size: %ld\n", idxmax * sizeof(T)); // type
    _data = _value;
    _idxwidth = 0;
    _idxmax = idxmax;
    _size = idxmax;
    _datatype = ZIPARRAY_FULLARRAY;

    if (oldvalue != NULL)
    {
        CopyDataFromOld(oldvalue, olddata, olddatatype, oldidxmax, oldidxwidth);
        Allocator::free(oldvalue);
    }
}

template <typename T>
void ZipArray<T>::InitHashMap(ZipArrayIdx_t idxmax)
{
    DEBUG("\n");
    void *oldvalue = _value;
    void *olddata = _data;
    int oldidxmax = _idxmax;
    int oldidxwidth = _idxwidth;
    int olddatatype = _datatype;

    _value = Allocator::Construct<ZipArrayMap_t>();
    _data = _value;
    _idxwidth = 0;
    _idxmax = idxmax;
    _size = idxmax;
    _datatype = ZIPARRAY_MAP;

    if (oldvalue != NULL)
    {
        CopyDataFromOld(oldvalue, olddata, olddatatype, oldidxmax, oldidxwidth);
        Allocator::free(oldvalue);
    }
}

inline static ZipArrayIdx_t PowerOf2(ZipArrayIdx_t idxmax)
{
    ZipArrayIdx_t v = idxmax;
    ZipArrayIdx_t o = 0;
    while (v >>= 1)
        ++o;

    if (idxmax > (1 << o))
        ++o;

    return 1 << o;
}

template <typename T>
void ZipArray<T>::Init(ZipArrayIdx_t idxmax, unsigned char targettype)
{
    DEBUG("type: %d\n", targettype);
    if (ZIPARRAY_FULLARRAY == targettype)
    {
        InitFull(idxmax);
        return;
    }

    if (ZIPARRAY_MAP == targettype)
    {
        InitHashMap(idxmax);
        return;
    }

    if (targettype != ZIPARRAY_ARRAY_1_8 && targettype != ZIPARRAY_ARRAY_1_4 && targettype != ZIPARRAY_ARRAY_1_2)
        return;

    if (targettype == ZIPARRAY_ARRAY_1_4 && _datatype != ZIPARRAY_ARRAY_1_8)
        return;

    if (targettype == ZIPARRAY_ARRAY_1_2 && _datatype != ZIPARRAY_ARRAY_1_4)
        return;

    _datatype = targettype;
    if (idxmax < ZIP_ARRAY_SIZE_MIN)
        idxmax = ZIP_ARRAY_SIZE_MIN;

    void *oldvalue = _value;
    void *olddata = _data;
    ZipArrayIdx_t oldidxmax = _idxmax;
    ZipArrayIdx_t oldidxwidth = _idxwidth;
    unsigned char olddatatype = _datatype;

    _idxmax = PowerOf2(idxmax);

    if (ZIPARRAY_ARRAY_1_8 == targettype)
        _size = _idxmax / 8;
    else if (ZIPARRAY_ARRAY_1_4 == targettype)
        _size = _idxmax / 4;
    else if (ZIPARRAY_ARRAY_1_2 == targettype)
        _size = _idxmax / 2;

    // 存储索引的位数
    _idxwidth = CalcIdxWidth(_size);

    // 存储索引位图的空间大小
    int mapsize = (_idxwidth * _idxmax + 7) / 8;
    int value_size = _size * sizeof(T); // targettype
    // 总的数据空间大小
    int total = mapsize + value_size;

    _value = Allocator::malloc(total);
    if (NULL == _value)
    {
        ERROR("malloc: %d\n", total);
        return;
    }

    _data = ((unsigned char *)_value) + mapsize;
    std::memset(_value, 0x00, total);
    DEBUG("total size: %d\n", total);

    if (oldvalue != NULL && _count > 0)
    {
        _count = 0; // Set的时候是基本_count来的，所以这里在复制原来数据时需要把_count清0
        CopyDataFromOld(oldvalue, olddata, olddatatype, oldidxmax, oldidxwidth);
        Allocator::free(oldvalue);
    }
}

template <typename T>
void ZipArray<T>::CopyDataFromOld(void *src, void *data, unsigned char datatype, ZipArrayIdx_t idxmax, unsigned char idxwidth)
{
    DEBUG("\n");
    for (ZipArrayIdx_t i = 0; i < idxmax; ++i)
    {
        T *v = NULL;
        if (HasData(i, &v, src, data, datatype, idxwidth))
            Set(i, *v);
    }
}

/**
 * idx 目标范围的索引值
 * map 存储位图的起始地址
 * idxwidth 存放实际空间索引值的位数
*/
inline static ZipArrayIdx_t GetRealIdx(ZipArrayIdx_t idx, void *map, unsigned char idxwidth)
{
    int bitpos = idx * idxwidth; // 目标范围索引在位图中位起始位
    int offset = bitpos >> 3;    // 在位图中的字节起始位置
    int start = bitpos % 8;      // 索引在当前字节中的起始位
    unsigned char mapvalue = ((unsigned char *)map)[offset];

    int realidx = -1;
    int retain = 8 - start;
    if (retain >= idxwidth) // 不需拼值
    {
        unsigned char m = ((((0xff << (retain - idxwidth)) & 0xff) >> (8 - idxwidth)) & 0xff) << start;
        realidx = (mapvalue & m) >> start;
    }
    else // 另一部分值在下一个字节里
    {
        // 一部分
        int left = idxwidth - retain;
        int realidx1 = (mapvalue >> start) & 0xff;
        unsigned char mapvalue2 = ((unsigned char *)map)[offset + 1];
        int realidx2 = (((mapvalue2 << (8 - left)) & 0xff) >> (8 - left - retain)) & 0xff;
        realidx = (realidx1 | realidx2) & 0xff;
    }

    //DEBUG("idx: %d, realidx: %d\n", idx, realidx);
    return realidx;
}

// must be empty
inline void SetRealIdx(ZipArrayIdx_t idx, ZipArrayIdx_t realidx, void *map, unsigned char idxwidth)
{
    //DEBUG("idx: %d, realidx: %d\n", idx, realidx);
    int bitpos = idx * idxwidth;
    int offset = bitpos >> 3;
    int start = bitpos % 8;
    unsigned char &mapvalue = ((unsigned char *)map)[offset];
    int retain = 8 - start; // 当前字节存储的索引值位数

    ZipArrayIdx_t idxvalue = realidx;
    if (retain >= idxwidth) // 装得下
    {
        idxvalue <<= start;
        mapvalue |= idxvalue;
    }
    else
    {
        unsigned char part1 = idxvalue << start;
        mapvalue |= part1;
        unsigned char &mapvalue2 = ((unsigned char *)map)[offset + 1];
        unsigned char part2 = (idxvalue >> retain) & 0xff;
        mapvalue2 |= part2;
    }
}

template <typename T>
T &ZipArray<T>::GetRef(ZipArrayIdx_t idx) const
{
    if (idx >= _idxmax - 1)
        return (T &)null;

    DEBUG("idx: %d\n", idx);
    if (ZIPARRAY_FULLARRAY == _datatype)
        return ((T *)_data)[idx];

    if (ZIPARRAY_MAP == _datatype)
        return (*((ZipArrayMap_t *)_data))[idx];

    ZipArrayIdx_t realidx = GetRealIdx(idx, _value, _idxwidth);
    if (realidx != 0)
        return ((T *)_data)[realidx - 1];

    return (T &)null;
}

template <typename T>
void ZipArray<T>::ConvertToHashMap()
{
    Init(_idxmax, ZIPARRAY_MAP);
}

template <typename T>
void ZipArray<T>::ConvertToFull()
{
    Init(_idxmax, ZIPARRAY_FULLARRAY);
}

template <typename T>
void ZipArray<T>::ConvertTo_1_4()
{
    Init(_idxmax, ZIPARRAY_ARRAY_1_4);
}

template <typename T>
void ZipArray<T>::ConvertTo_1_2()
{
    Init(_idxmax, ZIPARRAY_ARRAY_1_2);
}

template <typename T>
T ZipArray<T>::Clear(ZipArrayIdx_t idx)
{
    T *v = NULL;
    if (HasData(idx, &v, NULL))
        return *v;

    return T();
}

template <typename T>
T ZipArray<T>::Set(ZipArrayIdx_t idx, const T &value)
{
    if (_datatype != ZIPARRAY_MAP && _count >= ZIP_ARRAY_SIZE_MAX - 1)
    {
        ConvertToHashMap();
        return Set(idx, value);
    }

    int idxmax_1_8 = _idxmax / 8;
    if (ZIPARRAY_ARRAY_1_8 == _datatype && _count >= idxmax_1_8 - 1)
    {
        ConvertTo_1_4();
        return Set(idx, value);
    }

    int idxmax_1_4 = _idxmax / 4;
    if (ZIPARRAY_ARRAY_1_4 == _datatype && _count >= idxmax_1_4 - 1)
    {
        ConvertTo_1_2();
        return Set(idx, value);
    }

    int idxmax_1_2 = _idxmax / 2;
    if (ZIPARRAY_ARRAY_1_2 == _datatype && _count >= idxmax_1_2 - 1)
    {
        ConvertToFull();
        return Set(idx, value);
    }

    if (idx >= _idxmax - 1)
        return T();

    if (ZIPARRAY_FULLARRAY == _datatype)
    {
        T &v = ((T *)_data)[idx];
        T oldv = v;
        v = value;
        ++_count;
        return oldv;
    }

    if (ZIPARRAY_MAP == _datatype)
    {
        T &v = (*((ZipArrayMap_t *)_data))[idx];
        T oldv = v;
        v = value;
        ++_count;
        ++_size;
        return oldv;
    }

    // 当前字节存储的部分索引值，如果存得下则为整个细索引值，存不下另一部分在下一下字节
    ZipArrayIdx_t realidx = GetRealIdx(idx, _value, _idxwidth);
    if (realidx == 0) // 空
    {
        realidx = ++_count;
        SetRealIdx(idx, realidx, _value, _idxwidth);
    }

    T &v = ((T *)_data)[realidx - 1];
    T oldv = v;
    v = value;
    return oldv;
}

template <typename T>
bool ZipArray<T>::HasData(ZipArrayIdx_t idx, T **ppvalue, void *map, void *data, unsigned char datatype, unsigned char idxwidth)
{
    if (ZIPARRAY_FULLARRAY == datatype)
    {
        if (ppvalue != NULL)
            *ppvalue = &((T *)data)[idx];

        return true;
    }

    if (ZIPARRAY_MAP == datatype)
    {
        if (ppvalue != NULL)
            *ppvalue = &(*((ZipArrayMap_t *)data))[idx];

        return true;
    }

    ZipArrayIdx_t realidx = GetRealIdx(idx, map, idxwidth);
    if (realidx != 0)
    {
        if (ppvalue != NULL)
            *ppvalue = &((T *)data)[realidx - 1];

        return true;
    }

    return false;
}

template <typename T>
bool ZipArray<T>::HasData(ZipArrayIdx_t idx, T **ppvalue, ZipArrayIdx_t *ridx) const
{
    if (ZIPARRAY_FULLARRAY == _datatype)
    {
        if (ppvalue != NULL)
            *ppvalue = &((T *)_data)[idx];
        if (ridx != NULL)
            *ridx = idx;

        return true;
    }

    if (ZIPARRAY_MAP == _datatype)
    {
        if (ppvalue != NULL)
            *ppvalue = &(*((ZipArrayMap_t *)_data))[idx];
        if (ridx != NULL)
            *ridx = idx;

        return true;
    }

    ZipArrayIdx_t realidx = GetRealIdx(idx, _value, _idxwidth);
    if (realidx != 0)
    {
        if (ppvalue != NULL)
            *ppvalue = &((T *)_data)[realidx - 1];

        if (ridx != NULL)
            *ridx = realidx - 1;

        return true;
    }

    return false;
}
template <typename T>
template <typename U>
void ZipArray<T>::CopyFrom(const ZipArray<U> &other)
{
    if (other._datatype <= ZIPARRAY_FULLARRAY)
    {
        if (sizeof(U) == sizeof(T) &&
            ((is_float<T>::value == 1 && is_float<U>::value == 1) ||
             (is_integer<T>::value == 1 && is_integer<U>::value == 1)))
        {
            int mapsize = (_idxwidth * _idxmax + 7) / 8;
            int value_size = _size * sizeof(T);
            int total = mapsize + value_size;
            _value = Allocator::malloc(total);
            _data = (unsigned char *)_value + mapsize;
            std::memcpy(_value, other._value, total);
        }
        else
        {
            Init(other._idxmax, other._datatype);
            for (ZipArrayIdx_t i = 0; i < other._idxmax; ++i)
            {
                U *pv = NULL;
                if (other.HasData(i, &pv, NULL))
                    Set(i, *pv);
            }
        }
    }
    else
    {
        _count = other._count;
        _size = other._count;
        _idxmax = other._idxmax;
        _idxwidth = other._idxwidth;
        _datatype = other._datatype;
        _value = Allocator::Construct<ZipArrayMap_t>();
        _data = _value;
        *((ZipArrayMap_t *)_value) = *((ZipArrayMap_t *)other._value);
    }
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator=(const ZipArray<U> &other)
{
    Clear();

    _count = other._count;
    _size = other._size;
    _idxmax = other._idxmax;
    _idxwidth = other._idxwidth;
    _datatype = other._datatype;

    CopyFrom(other);
    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> ZipArray<T>::operator+(const ZipArray<U> &other)
{
    ZipArray<T> v(*this);
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (v.HasData(i, &pv, NULL))
                *pv += *opv;
            else
                v.Set(i, *opv);
        }
    }

    return v;
}

template <typename T>
template <typename U>
ZipArray<T> ZipArray<T>::operator-(const ZipArray<U> &other)
{
    ZipArray<T> v(*this);
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (v.HasData(i, &pv, NULL))
                *pv -= *opv;
            else
                v.Set(i, -*opv);
        }
    }

    return v;
}

template <typename T>
template <typename U>
ZipArray<T> ZipArray<T>::operator*(const ZipArray<U> &other)
{
    ZipArray<T> v(*this);
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (v.HasData(i, &pv, NULL))
                *pv *= *opv;
        }
    }

    return v;
}

template <typename T>
template <typename U>
ZipArray<T> ZipArray<T>::operator/(const ZipArray<U> &other)
{
    ZipArray<T> v(*this);
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (v.HasData(i, &pv, NULL) && *opv != 0)
                *pv /= *opv;
        }
    }

    return v;
}

template <typename T>
template <typename U>
ZipArray<T> ZipArray<T>::operator%(const ZipArray<U> &other)
{
    ZipArray<T> v(*this);
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (v.HasData(i, &pv, NULL) && *opv != 0)
                *pv %= *opv;
        }
    }

    return v;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator+=(const ZipArray<U> &other)
{
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (HasData(i, &pv, NULL))
                *pv += *opv;
            else
                Set(i, *opv);
        }
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator-=(const ZipArray<U> &other)
{
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (HasData(i, &pv, NULL))
                *pv -= *opv;
            else
                Set(i, -*opv);
        }
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator*=(const ZipArray<U> &other)
{
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (HasData(i, &pv, NULL))
                *pv *= *opv;
        }
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator/=(const ZipArray<U> &other)
{
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (HasData(i, &pv, NULL) && *opv != 0)
                *pv /= *opv;
        }
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator%=(const ZipArray<U> &other)
{
    for (int i = 0; i < other._idxmax; ++i)
    {
        T *pv = NULL;
        T *opv = NULL;
        if (other.HasData(i, &opv, NULL))
        {
            if (HasData(i, &pv, NULL) && *opv != 0)
                *pv %= *opv;
        }
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator+=(const U &v)
{
    for (int i = 0; i < _idxmax; ++i)
    {
        T *pv = NULL;
        if (HasData(i, &pv, NULL))
            *pv += v;
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator-=(const U &v)
{
    for (int i = 0; i < _idxmax; ++i)
    {
        T *pv = NULL;
        if (HasData(i, &pv, NULL))
            *pv -= v;
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator*=(const U &v)
{
    for (int i = 0; i < _idxmax; ++i)
    {
        T *pv = NULL;
        if (HasData(i, &pv, NULL))
            *pv *= v;
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator/=(const U &v)
{
    for (int i = 0; i < _idxmax; ++i)
    {
        T *pv = NULL;
        if (HasData(i, &pv, NULL) && v != 0)
            *pv /= v;
    }

    return *this;
}

template <typename T>
template <typename U>
ZipArray<T> &ZipArray<T>::operator%=(const U &v)
{
    for (int i = 0; i < _idxmax; ++i)
    {
        T *pv = NULL;
        if (HasData(i, &pv, NULL) && v != 0)
            *pv %= v;
    }

    return *this;
}

template <typename T>
void ZipArray<T>::Print()
{
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "count: " << _count << " size: " << _size << " idxmax: " << _idxmax << " idxwidth: " << (unsigned short)_idxwidth << " type: " << (unsigned short)_datatype << std::endl;
    for (ZipArrayIdx_t i = 0; i < _idxmax; ++i)
    {
        T *v = NULL;
        ZipArrayIdx_t realidx = -1;
        if (HasData(i, &v, &realidx))
        {
            std::cout << "i: " << i << " realidx: " << realidx << " value: " << *v << std::endl;
        }
    }
}

template <typename T>
void ZipArray<T>::Serialize(OStream &stream) const
{
    stream << _idxmax << _idxwidth << _datatype;
    size_t currpos = stream.Curr();
    if (stream.WriteSize() < sizeof(ZipArrayIdx_t))
        return;

    ZipArrayIdx_t c = 0;
    stream << (ZipArrayIdx_t)(0); // 先占位
    for (ZipArrayIdx_t i = 0; i < _idxmax; ++i)
    {
        T *v = NULL;
        ZipArrayIdx_t realidx = -1;
        if (HasData(i, &v, &realidx))
        {
            stream << i;
            stream << *v;
            ++c;
        }
    }

    stream.At<ZipArrayIdx_t>(currpos) = c;
}

template <typename T>
void ZipArray<T>::Unserialize(IStream &stream)
{
    ZipArrayIdx_t oldmax = _idxmax;
    ZipArrayIdx_t oldidxwidth = _idxwidth;
    unsigned char olddatatype = _datatype;

    stream >> _idxmax >> _idxwidth >> _datatype;
    if (_idxmax != oldmax || _idxwidth != oldidxwidth || olddatatype != _datatype)
        Init(_idxmax, _datatype);

    ZipArrayIdx_t c = 0;
    stream >> c;
    for (ZipArrayIdx_t i = 0; i < c; ++i)
    {
        ZipArrayIdx_t idx = 0;
        T v = T();
        stream >> idx;
        stream >> v;
        Set(idx, v);
    }
}

template <typename T>
const std::string ZipArray<T>::ToString() const
{
    std::stringstream ss;
    for (ZipArrayIdx_t i = 0; i < _idxmax; ++i)
    {
        T *v = NULL;
        ZipArrayIdx_t realidx = -1;
        if (HasData(i, &v, &realidx))
        {
            ss << " [" << i << "](" << realidx << ") = ";
            ss << *v << ", ";
        }
    }
    return ss.str();
}

template <typename T>
OStream &operator<<(OStream &stream, ZipArray<T> &ziparr)
{
    ziparr.Serialize(stream);
    return stream;
}

template <typename T>
IOStream &operator<<(IOStream &stream, ZipArray<T> &ziparr)
{
    ziparr.Serialize((OStream&)stream);
    return stream;
}

template <typename T>
IStream &operator>>(IStream &stream, ZipArray<T> &ziparr)
{
    ziparr.Unserialize(stream);
    return stream;
}

template <typename T>
IOStream &operator>>(IOStream &stream, ZipArray<T> &ziparr)
{
    ziparr.Unserialize((IStream&)stream);
    return stream;
}

template <typename T>
const T ZipArray<T>::null = T();

} // namespace XSpace

#endif // _ZIPARRAY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
