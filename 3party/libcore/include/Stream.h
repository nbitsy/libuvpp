
#ifndef _STREAM_H_
#define _STREAM_H_

#include <cstring>
#include <exception>
#include <iostream>

#include <vector>
#include <list>
#include <map>
#include <set>

namespace XSpace
{

static const int APPEND_STR_LEN = 0x01;
static const int APPEND_STR_NULL = 0x02;
static const int K_TRUNK_SIZE = (64 * 1024);

typedef unsigned short strlen_t;
typedef unsigned short datalen_t;

typedef std::vector<unsigned char> DefaultContainer;

template <typename C = DefaultContainer>
class StreamBase
{
public:
    typedef unsigned int size_type;
    typedef C container;
    typedef typename container::pointer pointer;
    typedef typename container::const_pointer const_pointer;
    typedef typename container::value_type value_type;

public:
    explicit StreamBase(size_type size, size_type curr = 0, size_type off = 0)
        : _data(size), _wpos(curr), _rpos(off), _max(size), _outofdata(false)
    {
    }

    StreamBase(pointer data, size_type size, size_type curr = 0, size_type off = 0)
        : _data(data), _wpos(curr), _rpos(off), _max(size), _outofdata(false)
    {
    }

    // for virtual inherit
    StreamBase()
        : _data(K_TRUNK_SIZE), _wpos(0), _rpos(0), _max(K_TRUNK_SIZE), _outofdata(false)
    {
    }

    ~StreamBase() {}

    inline size_type ReadSize() const { return _wpos - _rpos; }
    inline size_type WriteSize() const { return _max - _wpos; }

    inline bool Empty() const { return _wpos == _rpos; }
    inline void Clear() { _rpos = _wpos = 0; }

    // XXX: DANGEROUS, YOU MUST KNOW WHAT YOU ARE DOING
    inline bool WriteFlip(size_type size)
    {
        _wpos += size;
        if (_wpos > _max)
        {
            _wpos = _max;
            return false;
        }
        return true;
    }

    inline bool ReadFlipSilence(size_type size)
    {
        _rpos += size;
        if (_rpos > _wpos)
        {
            _rpos = _wpos;
            _outofdata = true;
            return false;
        }
        return true;
    }

    inline bool CheckSize(size_type size)
    {
        Resize(size);
        return true;
    }

    inline void Resize(size_type size)
    {
        if (WriteSize() < (size + 8))
        {
            unsigned int nsize = Max() + K_TRUNK_SIZE * ((K_TRUNK_SIZE + size + 8) / K_TRUNK_SIZE);
            _data.resize(nsize, 0); // XXX: data copyed
            if (_data.size() == nsize)
                _max = nsize;
        }
    }

    inline size_type Curr() const { return _wpos; }
    inline size_type Off() const { return _rpos; }
    inline size_type Max() const { return _max; }

    inline bool IsOutOfData() const { return _outofdata; }

protected:
    container _data;
    size_type _wpos;
    size_type _rpos;
    size_type _max;
    bool _outofdata;
};

template <typename Container = DefaultContainer>
class InStream : public virtual StreamBase<Container>
{
public:
    typedef StreamBase<Container> Base;
    typedef typename Container::const_pointer const_pointer;
    typedef typename Container::pointer pointer;
    typedef typename Container::size_type size_type;
    typedef typename Container::value_type value_type;

    using Base::_data;
    using Base::_max;
    using Base::_outofdata;
    using Base::_rpos;
    using Base::_wpos;
    using Base::ReadSize;
    using Base::WriteSize;

public:
    explicit InStream(size_t size) : StreamBase<Container>(size)
    {
    }

    explicit InStream(pointer data, size_type size) : StreamBase<Container>(data, size)
    {
    }

    ~InStream()
    {
    }

    inline const_pointer ReadPos() const { return &_data[_rpos]; }
    inline pointer ReadPos() { return &_data[_rpos]; }

    inline const_pointer WritePos() const { return &_data[_wpos]; }
    inline pointer WritePos() { return &_data[_wpos]; }

    inline char *C_ReadPos() { return reinterpret_cast<char *>(&_data[_rpos]); }
    inline char *C_WritePos() { return reinterpret_cast<char *>(&_data[_wpos]); }

    inline const char *C_ReadPos() const { return reinterpret_cast<const char *>(&_data[_rpos]); }
    inline const char *C_WritePos() const { return reinterpret_cast<const char *>(&_data[_wpos]); }

    InStream &Read(void *v, size_type c)
    {
        if (!ReadSize())
        {
            _outofdata = true;
            return *this; // XXX: no data read
        }

        if (ReadSize() >= c)
        {
            _outofdata = false;
            std::memcpy(v, &_data[_rpos], c);
            _rpos += c;
        }
        else
        {
            // XXX: no data read
        }

        return *this;
    }

#define READ(t, v)                                 \
    if (ReadSize() >= sizeof(v))                   \
    {                                              \
        _outofdata = false;                        \
        v = *reinterpret_cast<t *>(&_data[_rpos]); \
        _rpos += sizeof(v);                        \
    }                                              \
    else                                           \
    {                                              \
        _outofdata = true;                         \
    }                                              \
    return *this;

    inline InStream &Read(bool &v)
    {
        READ(bool, v);
    }
    inline InStream &Read(char &v) { READ(char, v); }
    inline InStream &Read(unsigned char &v) { READ(unsigned char, v); }
    inline InStream &Read(short &v) { READ(short, v); }
    inline InStream &Read(unsigned short &v) { READ(unsigned short, v); }
    inline InStream &Read(int &v) { READ(int, v); }
    inline InStream &Read(unsigned int &v) { READ(unsigned int, v); }
    inline InStream &Read(long &v) { READ(long, v); }
    inline InStream &Read(unsigned long &v) { READ(unsigned long, v); }
    inline InStream &Read(long long &v) { READ(long long, v); }
    inline InStream &Read(unsigned long long &v) { READ(unsigned long long, v); }
    inline InStream &Read(float &v) { READ(float, v); }
    inline InStream &Read(double &v) { READ(double, v); }
    inline InStream &Read(long double &v) { READ(long double, v); }

#undef READ

    inline const value_type &At(size_type i) const
    {
        if ((i + _rpos) >= _max)
        {
            static value_type t; // XXX: mult thread unsafe
            return t;
        }
        return *&_data[i + _rpos];
    }

    // XXX: Do not save the reference which returned by at()
    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
#if defined(CXX0X) && CXX0X
    template <typename U = value_type>
#else
    template <typename U>
#endif
    inline const U &At(size_type i = 0) const
    {
        if ((i + _rpos) >= _max || (WriteSize() < sizeof(U)))
        {
            static U u; // XXX: mult thread unsafe
            return u;
        }
        return *reinterpret_cast<U *>(&_data[i + _rpos]);
    }

    inline const value_type &operator[](size_type i) const
    {
        if (i + _rpos >= _max)
        {
            static value_type t; // XXX: mult thread unsafe
            return t;
        }
        return *&_data[i + _rpos];
    }

    void ReadFlip(size_type size = 0)
    {
        _rpos += size;
        if (_rpos < _wpos)
        {
            if (_rpos) // XXX: move the data to the start of the buffer
            {
                size_type c = _wpos - _rpos;
                std::memcpy(&_data[0], &_data[_rpos], c);
                _rpos = 0;
                _wpos = c;
            }
        }
        else
        {
            _rpos = _wpos = 0;
        }
    }

    inline InStream &operator>>(bool &v) { return Read(v); }
    inline InStream &operator>>(char &v) { return Read(v); }
    inline InStream &operator>>(unsigned char &v) { return Read(v); }
    inline InStream &operator>>(short &v) { return Read(v); }
    inline InStream &operator>>(unsigned short &v) { return Read(v); }
    inline InStream &operator>>(int &v) { return Read(v); }
    inline InStream &operator>>(unsigned int &v) { return Read(v); }
    inline InStream &operator>>(long &v) { return Read(v); }
    inline InStream &operator>>(unsigned long &v) { return Read(v); }
    inline InStream &operator>>(long long &v) { return Read(v); }
    inline InStream &operator>>(unsigned long long &v) { return Read(v); }
    inline InStream &operator>>(float &v) { return Read(v); }
    inline InStream &operator>>(double &v) { return Read(v); }
    inline InStream &operator>>(long double &v) { return Read(v); }
    inline InStream &operator>>(std::string &str)
    {
        strlen_t len = 0;
        Read(len);
        if (len != 0)
        {
            str.resize(len);
            Read(&str[0], len);
        }
        return *this;
    }
    template <typename K, typename V>
    inline InStream &operator>>(std::pair<K, V> &v)
    {
        (*this) >> v.first >> v.second;
        return *this;
    }
    template <typename T>
    inline InStream &operator>>(std::vector<T> &v)
    {
        datalen_t len = 0;
        (*this) >> len;
        if (0 == len)
            return *this;

        v.resize(len);
        for (datalen_t i = 0; i < len; ++i)
            (*this) >> v[i];

        return *this;
    }
    template <typename T>
    inline InStream &operator>>(std::list<T> &v)
    {
        datalen_t len = 0;
        (*this) >> len;
        if (0 == len)
            return *this;

        for (datalen_t i = 0; i < len; ++i)
        {
            v.emplace_back();
            T &tv = v.back();
            (*this) >> tv;
        }

        return *this;
    }
    template <typename T, typename C = std::less<T>>
    inline InStream &operator>>(std::set<T, C> &v)
    {
        datalen_t len = 0;
        (*this) >> len;
        if (0 == len)
            return *this;

        T tv;
        for (datalen_t i = 0; i < len; ++i)
        {
            (*this) >> tv;
            v.insert(tv);
        }

        return *this;
    }
    template <typename K, typename V, typename C = std::less<K>>
    inline InStream &operator>>(std::map<K, V, C> &v)
    {
        datalen_t len = 0;
        (*this) >> len;
        if (0 == len)
            return *this;

        std::pair<K, V> tv;
        for (datalen_t i = 0; i < len; ++i)
        {
            (*this) >> tv;
            v.insert(tv);
        }

        return *this;
    }
};

template <typename Container = DefaultContainer>
class OutStream : public virtual StreamBase<Container>
{
public:
    typedef StreamBase<Container> Base;
    typedef typename Container::const_pointer const_pointer;
    typedef typename Container::pointer pointer;
    typedef typename Container::size_type size_type;
    typedef typename Container::value_type value_type;

    using Base::_data;
    using Base::_max;
    using Base::_outofdata;
    using Base::_rpos;
    using Base::_wpos;
    using Base::CheckSize;
    using Base::ReadSize;
    using Base::WriteSize;

public:
    explicit OutStream(size_t size) : StreamBase<Container>(size)
    {
    }

    explicit OutStream(pointer data, size_type size) : StreamBase<Container>(data, size)
    {
    }

    ~OutStream()
    {
    }

    inline OutStream &Put(const void *buf, size_type size, unsigned char lennull = 0)
    {
        return Append(buf, size, lennull);
    }

    // XXX: do not call remove() frequently
    void Remove(size_t s, size_t n)
    {
        if ((_rpos + s) >= _max)
            return;
        if ((_rpos + s + n) > _max)
            n = _max - s - _rpos;

        if ((_rpos + s + n) >= _wpos)
        {
            _wpos = (_rpos + s);
        }
        else
        {
            std::memmove(&_data[0] + _rpos + s, &_data[0] + _rpos + s + n, n);
            _wpos -= n;
        }
    }

    inline value_type &At(size_type i)
    {
        if ((i + _rpos) >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i + _rpos];
    }

#if defined(CXX0X) && CXX0X
    template <typename U = value_type>
#else
    template <typename U>
#endif
    inline U &At(size_type i = 0)
    {
        if ((i + _rpos) >= _max || (WriteSize() < sizeof(U))) // XXX:
        {
            static U u;
            return u;
        }
        return *reinterpret_cast<U *>(&_data[i + _rpos]);
    }

    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
    inline value_type &operator[](size_type i)
    {
        if ((i + _rpos) >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i + _rpos];
    }

#define WRITE(t, v)                            \
    if (!CheckSize(sizeof(v)))                 \
        return *this;                          \
    *reinterpret_cast<t *>(&_data[_wpos]) = v; \
    _wpos += sizeof(v);                        \
    return *this;

    inline OutStream &Write(bool v)
    {
        WRITE(bool, v);
    }
    inline OutStream &Write(char v) { WRITE(char, v); }
    inline OutStream &Write(unsigned char v) { WRITE(unsigned char, v); }
    inline OutStream &Write(short v) { WRITE(short, v); }
    inline OutStream &Write(unsigned short v) { WRITE(unsigned short, v); }
    inline OutStream &Write(int v) { WRITE(int, v); }
    inline OutStream &Write(unsigned int v) { WRITE(unsigned int, v); }
    inline OutStream &Write(long v) { WRITE(long, v); }
    inline OutStream &Write(unsigned long v) { WRITE(unsigned long, v); }
    inline OutStream &Write(long long v) { WRITE(long long, v); }
    inline OutStream &Write(unsigned long long v) { WRITE(unsigned long long, v); }
    inline OutStream &Write(float v) { WRITE(float, v); }
    inline OutStream &Write(double v) { WRITE(double, v); }
    inline OutStream &Write(long double v) { WRITE(long double, v); }
#undef WRITE

    OutStream &Append(const void *buf, size_type size, unsigned char lennull = 0)
    {
        if (lennull != 0)
        {
            size_type nsize = size;
            if (lennull & APPEND_STR_LEN) // XXX: size must be less or equal to 0xffff
            {
                if (size > 0xffff)
                    return *this;
                nsize += sizeof(strlen_t);
            }
            if (lennull & APPEND_STR_NULL)
                nsize += 1;
            if (!CheckSize(nsize))
                return *this;

            if (lennull & APPEND_STR_LEN)
            {
                // XXX: strlen_t to store size
                if (lennull & APPEND_STR_NULL)
                    *reinterpret_cast<strlen_t *>(&_data[_wpos]) = size + 1;
                else
                    *reinterpret_cast<strlen_t *>(&_data[_wpos]) = size;
                _wpos += sizeof(strlen_t);
                std::memcpy(&_data[_wpos], buf, size);
                _wpos += size;
                if (lennull & APPEND_STR_NULL)
                    _data[_wpos++] = '\0';
            }
            else if (lennull & APPEND_STR_NULL)
            {
                std::memcpy(&_data[_wpos], buf, size);
                _wpos += size;
                _data[_wpos++] = '\0';
            }
        }
        else
        {
            if (!CheckSize(size))
                return *this;
            std::memcpy(&_data[_wpos], buf, size);
            _wpos += size;
        }
        return *this;
    }

    inline OutStream &operator<<(bool v) { return Write(v); }
    inline OutStream &operator<<(char v) { return Write(v); }
    inline OutStream &operator<<(unsigned char v) { return Write(v); }
    inline OutStream &operator<<(short v) { return Write(v); }
    inline OutStream &operator<<(unsigned short v) { return Write(v); }
    inline OutStream &operator<<(int v) { return Write(v); }
    inline OutStream &operator<<(unsigned v) { return Write(v); }
    inline OutStream &operator<<(long v) { return Write(v); }
    inline OutStream &operator<<(unsigned long v) { return Write(v); }
    inline OutStream &operator<<(long long v) { return Write(v); }
    inline OutStream &operator<<(unsigned long long v) { return Write(v); }
    inline OutStream &operator<<(float v) { return Write(v); }
    inline OutStream &operator<<(double v) { return Write(v); }
    inline OutStream &operator<<(long double v) { return Write(v); }

    inline OutStream &operator<<(const char *str)
    {
        if (!str)
            return *this;
        strlen_t len = static_cast<strlen_t>(strlen(str));
        return Append(str, len, APPEND_STR_LEN);
    }

    inline OutStream &operator<<(const std::string &str)
    {
        return (*this) << (str.c_str());
    }

    template <typename K, typename V>
    inline OutStream &operator<<(const std::pair<K, V> &v)
    {
        (*this) << v.first << v.second;
        return *this;
    }
    template <typename T>
    inline OutStream &operator<<(const std::vector<T> &v)
    {
        datalen_t len = static_cast<datalen_t>(v.size());
        (*this) << len;
        for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i)
            (*this) << *i;
        return *this;
    }
    template <typename T>
    inline OutStream &operator<<(const std::list<T> &v)
    {
        datalen_t len = static_cast<datalen_t>(v.size());
        (*this) << len;
        for (typename std::list<T>::const_iterator i = v.begin(); i != v.end(); ++i)
            (*this) << *i;
        return *this;
    }
    template <typename T, typename C = std::less<T>>
    inline OutStream &operator<<(const std::set<T, C> &v)
    {
        datalen_t len = static_cast<datalen_t>(v.size());
        (*this) << len;
        for (typename std::set<T, C>::const_iterator i = v.begin(); i != v.end(); ++i)
            (*this) << *i;
        return *this;
    }
    template <typename K, typename V, typename C = std::less<K>>
    inline OutStream &operator<<(const std::map<K, V, C> &v)
    {
        datalen_t len = static_cast<datalen_t>(v.size());
        (*this) << len;
        for (typename std::map<K, V, C>::const_iterator i = v.begin(); i != v.end(); ++i)
            (*this) << *i;
        return *this;
    }
};

template <typename Container = DefaultContainer>
class Stream : public InStream<Container>, public OutStream<Container>
{
public:
    explicit Stream(size_t size = 0)
        : StreamBase<Container>(size), InStream<Container>(size), OutStream<Container>(size)
    {
    }

    ~Stream()
    {
    }
};

template <typename T, unsigned int S = 8>
class Array
{
public:
    typedef unsigned int size_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T value_type;

public:
    explicit Array(size_type)
    {
        // do nothing
    }

    inline const T &operator[](size_type idx) const
    {
        return _data[idx];
    }

    inline T &operator[](size_type idx)
    {
        return _data[idx];
    }

    inline void resize(size_type, const T &)
    {
        // do nothing
    }

    inline size_type size() const
    {
        return S;
    }

private:
    T _data[S];
};

template <unsigned int S = 64>
class StackStream : public InStream<Array<unsigned char, S>>, public OutStream<Array<unsigned char, S>>
{
public:
    typedef Array<unsigned char, S> Container;

public:
    explicit StackStream(size_t = 0)
        : StreamBase<Container>(S), InStream<Container>(S), OutStream<Container>(S)
    {
    }

    ~StackStream()
    {
    }
};

template <typename T>
class MemHolder
{
public:
    typedef unsigned int size_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T value_type;

public:
    explicit MemHolder(pointer data) : _data(data), _size(0)
    {
    }

    explicit MemHolder(pointer data, size_type size) : _data(data), _size(size)
    {
    }

    inline const T &operator[](size_type idx) const
    {
        return _data[idx];
    }

    inline T &operator[](size_type idx)
    {
        return _data[idx];
    }

    inline void resize(size_type, const T &)
    {
        // do nothing
    }

    inline size_type size() const
    {
        return _size;
    }

private:
    pointer _data;
    size_type _size;
};

class OStream : public OutStream<>
{
};
class IStream : public InStream<>
{
};
class IOStream : public Stream<>
{
};

class MemStream : public InStream<MemHolder<unsigned char>>, public OutStream<MemHolder<unsigned char>>
{
public:
    typedef MemHolder<unsigned char> Container;
    using InStream<MemHolder<unsigned char>>::ReadSize;
    using OutStream<MemHolder<unsigned char>>::WriteSize;

public:
    explicit MemStream(unsigned char *data, unsigned int size, unsigned int wpos = 0)
        : StreamBase<Container>(data, size, wpos), InStream<Container>(data, size), OutStream<Container>(data, size)
    {
    }

    ~MemStream()
    {
    }
};

} // namespace XSpace

#endif // _STREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
