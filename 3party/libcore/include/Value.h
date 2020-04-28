
#ifndef _VALUE_H_
#define _VALUE_H_

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Stream.h"
#include "TypeTraits.h"

namespace XSpace
{

#define V_NULL 0x00
#define V_BOOL 0x01

// 有符号
#define V_CHAR 0x02
#define V_SHORT 0x03
#define V_INT 0x04
#define V_LONG 0x05
#define V_LONGLONG 0x06

// 无符号
#define V_UCHAR 0x7
#define V_USHORT 0x08
#define V_UINT 0x09
#define V_ULONG 0x0a
#define V_ULONGLONG 0x0b

// 浮点
#define V_FLOAT 0x0c
#define V_DOUBLE 0x0d

// 指针
#define V_POINTER 0x0e

// TODO: 目前只支持基础数据
#define V_STR 0x0f
#define V_VEC 0x10
#define V_SET 0x11
#define V_MAP 0x12
#define V_LIST 0x13
#define V_CLASS 0x14
#define V_VAL 0x15

struct ValueBase
{
    ValueBase() : Dirty(0), SyncType(0), Type(V_NULL), PVoid(0) {}
    ValueBase(const ValueBase &other) : Dirty(0), SyncType(0), Type(V_NULL), PVoid(0)
    {
        if (other.Type <= V_DOUBLE)
        {
            this->PVoid = other.PVoid;
            this->Type = other.Type;
            this->Dirty = true;
        }
    }

    ValueBase &operator=(const ValueBase &other)
    {
        if (other.Type <= V_DOUBLE)
        {
            this->PVoid = other.PVoid;
            this->Type = other.Type;
            this->Dirty = true;
        }
        return *this;
    }

    inline void Clearn() { this->Dirty = false; }

    template <typename T>
    T GetValue() const
    {
        switch (Type)
        {
        case V_BOOL:
            return BValue;
        case V_CHAR:
            return CValue;
        case V_UCHAR:
            return UCValue;
        case V_SHORT:
            return SValue;
        case V_USHORT:
            return USValue;
        case V_INT:
            return IValue;
        case V_UINT:
            return UIValue;
        case V_LONG:
            return LValue;
        case V_ULONG:
            return ULValue;
        case V_LONGLONG:
            return LLValue;
        case V_ULONGLONG:
            return ULLValue;
        case V_FLOAT:
            return FValue;
        case V_DOUBLE:
            return DValue;
        default:
            break;
        }
        return 0;
    }

    // 是否被修改过，被修改过后设置为1，则为脏数据
    unsigned char Dirty : 1;
    // 同步方式，应用层去解析
    unsigned char SyncType : 2;
    // 32种数据类型
    unsigned char Type : 5;

    // 真实值
    union {
        bool BValue;
        char CValue;
        short SValue;
        int IValue;
        long LValue;
        long long LLValue;

        unsigned char UCValue;
        unsigned short USValue;
        unsigned int UIValue;
        unsigned long ULValue;
        unsigned long long ULLValue;

        float FValue;
        double DValue;
        void *PVoid;
    };
};

struct Value;

template <typename V>
struct GetType
{
    static const int Value = V_NULL;
};
template <>
struct GetType<Value>
{
    static const int Value = V_VAL;
};
template <>
struct GetType<bool>
{
    static const int Value = V_BOOL;
};
template <>
struct GetType<char>
{
    static const int Value = V_CHAR;
};
template <>
struct GetType<unsigned char>
{
    static const int Value = V_UCHAR;
};
template <>
struct GetType<short>
{
    static const int Value = V_SHORT;
};
template <>
struct GetType<unsigned short>
{
    static const int Value = V_USHORT;
};
template <>
struct GetType<int>
{
    static const int Value = V_INT;
};
template <>
struct GetType<unsigned int>
{
    static const int Value = V_UINT;
};
template <>
struct GetType<long>
{
    static const int Value = V_LONG;
};
template <>
struct GetType<unsigned long>
{
    static const int Value = V_ULONG;
};
template <>
struct GetType<long long>
{
    static const int Value = V_LONGLONG;
};
template <>
struct GetType<unsigned long long>
{
    static const int Value = V_ULONGLONG;
};
template <>
struct GetType<float>
{
    static const int Value = V_FLOAT;
};
template <>
struct GetType<double>
{
    static const int Value = V_DOUBLE;
};
template <>
struct GetType<std::string>
{
    static const int Value = V_STR;
};

template <typename V>
void AssignValue(ValueBase &vb, const V &v) {}
template <>
void AssignValue<bool>(ValueBase &vb, const bool &v) { vb.BValue = v; }
template <>
void AssignValue<char>(ValueBase &vb, const char &v) { vb.CValue = v; }
template <>
void AssignValue<unsigned char>(ValueBase &vb, const unsigned char &v) { vb.UCValue = v; }
template <>
void AssignValue<short>(ValueBase &vb, const short &v) { vb.SValue = v; }
template <>
void AssignValue<unsigned short>(ValueBase &vb, const unsigned short &v) { vb.USValue = v; }
template <>
void AssignValue<int>(ValueBase &vb, const int &v) { vb.IValue = v; }
template <>
void AssignValue<unsigned int>(ValueBase &vb, const unsigned int &v) { vb.UIValue = v; }
template <>
void AssignValue<long>(ValueBase &vb, const long &v) { vb.LValue = v; }
template <>
void AssignValue<unsigned long>(ValueBase &vb, const unsigned long &v) { vb.ULValue = v; }
template <>
void AssignValue<long long>(ValueBase &vb, const long long &v) { vb.LLValue = v; }
template <>
void AssignValue<unsigned long long>(ValueBase &vb, const unsigned long long &v) { vb.ULLValue = v; }
template <>
void AssignValue<float>(ValueBase &vb, const float &v) { vb.FValue = v; }
template <>
void AssignValue<double>(ValueBase &vb, const double &v) { vb.DValue = v; }
template <>
void AssignValue<void *>(ValueBase &vb, void *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<char *>(ValueBase &vb, char *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<unsigned char *>(ValueBase &vb, unsigned char *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<int *>(ValueBase &vb, int *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<unsigned int *>(ValueBase &vb, unsigned int *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<long *>(ValueBase &vb, long *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<unsigned long *>(ValueBase &vb, unsigned long *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<long long *>(ValueBase &vb, long long *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<unsigned long long *>(ValueBase &vb, unsigned long long *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<float *>(ValueBase &vb, float *const &v) { vb.PVoid = (void *)v; }
template <>
void AssignValue<double *>(ValueBase &vb, double *const &v) { vb.PVoid = (void *)v; }

struct Value
{
    Value() : Val() {}

    template <typename V>
    explicit Value(const V &v) : Val()
    {
        AssignValue(Val, v);
        Val.Type = GetType<typename remove_const<V>::type>::Value;
    }

    Value(const Value &other) : Val(other.Val)
    {
    }

    Value &operator=(const Value &other)
    {
        Val = other.Val;
        return *this;
    }

    inline bool IsDirty() const { return Val.Dirty; }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << "Dirty: " << (Val.Dirty ? "true" : "false") << " Type: " << (int)Val.Type << " Value: ";
        if (Val.Type <= V_LONGLONG)
        {
            ss << Val.GetValue<long long>();
        }
        else if (Val.Type <= V_ULONGLONG)
        {
            ss << Val.GetValue<unsigned long long>();
        }
        else if (Val.Type <= V_DOUBLE)
        {
            ss << Val.GetValue<double>();
        }

        return ss.str();
    }

    ValueBase Val;
};

static inline IOStream &operator<<(IOStream &stream, const Value &v)
{
    stream << v.Val.ULValue;
    return stream;
}

static inline OStream &operator<<(OStream &stream, const Value &v)
{
    stream << v.Val.ULValue;
    return stream;
}

static inline IOStream &operator>>(IOStream &stream, Value &v)
{
    stream >> v.Val.ULLValue;
    v.Val.Dirty = true;
    return stream;
}

static inline IStream &operator>>(IStream &stream, Value &v)
{
    stream >> v.Val.ULLValue;
    v.Val.Dirty = true;
    return stream;
}

static inline std::stringstream &operator<<(std::stringstream &stream, const Value &v)
{
    stream << v.ToString();
    return stream;
}

static inline std::ostringstream &operator<<(std::ostringstream &stream, const Value &v)
{
    stream << v.ToString();
    return stream;
}

static inline std::iostream &operator<<(std::iostream &stream, const Value &v)
{
    stream << v.ToString();
    return stream;
}

static inline std::ostream &operator<<(std::ostream &stream, const Value &v)
{
    stream << v.ToString();
    return stream;
}

} // namespace XSpace

#endif // _VALUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
