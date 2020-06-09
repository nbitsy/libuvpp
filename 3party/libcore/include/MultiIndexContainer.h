
#ifndef _MULTIINDEXCONTAINER_H_
#define _MULTIINDEXCONTAINER_H_

#include <unordered_map>

#include "Debugger.h"
#include "TypeTraits.h"

namespace XSpace
{

class NullType1
{
public:
    NullType1() {}
    NullType1(const NullType1&) {}
};

class NullType2
{
public:
    NullType2() {}
    NullType2(const NullType2&) {}
};

template <typename K, typename V>
class EntryContainer
{
public:
    typedef typename remove_const<K>::type key_type;
    typedef typename remove_const<V>::type value_type;
    typedef const key_type const_key_type;
    typedef const value_type const_value_type;
    typedef key_type& key_reference;
    typedef const key_type& const_key_reference;
    typedef value_type& value_reference;
    typedef const value_type& const_value_reference;

public:
    EntryContainer() {}
    virtual ~EntryContainer() {}

    bool Add(const_key_reference key, const_value_reference value)
    {
        return _container.insert(std::make_pair(key, value)).second;
    }

    bool Remove(const_key_reference key)
    {
        auto i = _container.find(key);
        if (i == _container.end())
            return false;

        _container.erase(i);
        return true;
    }

    bool HasEntry(const_key_reference key) const
    {
        return _container.find(key) != _container.end();
    }

    const_value_reference Find(const_key_reference key) const
    {
        auto i = _container.find(key);
        if (i != _container.end())
            return i->second;

        static value_type null;
        return null;
    }

private:
    std::unordered_map<key_type, value_type> _container;
};

template <typename T>
class EntryContainer<NullType1, T>
{
public:
    typedef typename remove_const<NullType1>::type key_type;
    typedef typename remove_const<T>::type value_type;
    typedef const key_type const_key_type;
    typedef const value_type const_value_type;
    typedef key_type& key_reference;
    typedef const key_type& const_key_reference;
    typedef value_type& value_reference;
    typedef const value_type& const_value_reference;

    bool Add(const_key_reference key, const_value_reference value) { return true; }
    bool Remove(const_key_reference key) { return true; }
};

template <typename T>
class EntryContainer<NullType2, T>
{
public:
    typedef typename remove_const<NullType2>::type key_type;
    typedef typename remove_const<T>::type value_type;
    typedef const key_type const_key_type;
    typedef const value_type const_value_type;
    typedef key_type& key_reference;
    typedef const key_type& const_key_reference;
    typedef value_type& value_reference;
    typedef const value_type& const_value_reference;

    bool Add(const_key_reference key, const_value_reference value) { return true; }
    bool Remove(const_key_reference key) { return true; }
};

template <typename V, typename K1, typename K2, typename K3 = NullType1, typename K4 = NullType2>
class MultiIdxContainer
{
    typedef EntryContainer<K1, V> C1;
    typedef EntryContainer<K2, V> C2;
    typedef EntryContainer<K3, V> C3;
    typedef EntryContainer<K4, V> C4;
    typedef typename C1::value_reference value_reference;
    typedef typename C1::const_value_reference const_value_reference;

public:
    MultiIdxContainer() {}
    virtual ~MultiIdxContainer() {}

    bool Add(const_value_reference value,
             typename C1::const_key_reference key1,
             typename C2::const_key_reference key2,
             typename C3::const_key_reference key3 = typename C3::key_type(),
             typename C4::const_key_reference key4 = typename C4::key_type())
    {
        if (_c1.Add(key1, value))
        {
            if (_c2.Add(key2, value))
            {
                if (_c3.Add(key3, value))
                {
                    if (_c4.Add(key4, value))
                        return true;
                    goto _rm3;
                }
                goto _rm2;
            }

            goto _rm1;
        }
        else
        {
            return false;
        }

    _rm3:
        _c3.Remove(key3);
    _rm2:
        _c2.Remove(key2);
    _rm1:
        _c1.Remove(key1);
        return false;
    }

    bool AddByKey1(typename C1::const_key_reference key, value_reference value) { return _c1.Add(key, value); }
    bool AddByKey2(typename C2::const_key_reference key, value_reference value) { return _c2.Add(key, value); }
    bool AddByKey3(typename C3::const_key_reference key, value_reference value) { return _c3.Add(key, value); }
    bool AddByKey4(typename C4::const_key_reference key, value_reference value) { return _c4.Add(key, value); }

    // 只能从单个容器中删除，如果需要同步删除则需要提供更多的Key
    bool RemoveByKey1(typename C1::const_key_reference key) { return _c1.Remove(key); }
    bool RemoveByKey2(typename C2::const_key_reference key) { return _c2.Remove(key); }
    bool RemoveByKey3(typename C3::const_key_reference key) { return _c3.Remove(key); }
    bool RemoveByKey4(typename C4::const_key_reference key) { return _c4.Remove(key); }

    int Remove(typename C1::const_key_reference key1,
               typename C2::const_key_reference key2 = typename C2::key_type(),
               typename C3::const_key_reference key3 = typename C3::key_type(),
               typename C4::const_key_reference key4 = typename C4::key_type())
    {
        int c = _c1.Remove(key1);
        c += _c2.Remove(key2);
        c += _c3.Remove(key3);
        c += _c4.Remove(key4);
        return c;
    }

    inline const_value_reference FindByKey1(typename C1::const_key_reference key1) const { return _c1.Find(key1); }
    inline const_value_reference FindByKey2(typename C2::const_key_reference key2) const { return _c2.Find(key2); }
    inline const_value_reference FindByKey3(typename C3::const_key_reference key3) const { return _c3.Find(key3); }
    inline const_value_reference FindByKey4(typename C4::const_key_reference key4) const { return _c3.Find(key4); }

private:
    EntryContainer<K1, V> _c1;
    EntryContainer<K2, V> _c2;
    EntryContainer<K3, V> _c3;
    EntryContainer<K4, V> _c4;
};

} // namespace XSpace

#endif // _MULTIINDEXCONTAINER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
