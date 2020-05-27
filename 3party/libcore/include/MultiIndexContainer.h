
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
class MultiIdxContainer : public EntryContainer<K1, V>, public EntryContainer<K2, V>, public EntryContainer<K3, V>, public EntryContainer<K4, V>
{
    typedef EntryContainer<K1, V> Base1;
    typedef EntryContainer<K2, V> Base2;
    typedef EntryContainer<K3, V> Base3;
    typedef EntryContainer<K4, V> Base4;
    typedef typename Base1::value_reference value_reference;
    typedef typename Base1::const_value_reference const_value_reference;

public:
    MultiIdxContainer() {}
    virtual ~MultiIdxContainer() {}

    bool Add(const_value_reference value,
             typename Base1::const_key_reference key1,
             typename Base2::const_key_reference key2,
             typename Base3::const_key_reference key3 = typename Base3::key_type(),
             typename Base4::const_key_reference key4 = typename Base4::key_type())
    {
        if (Base1::Add(key1, value))
        {
            if (Base2::Add(key2, value))
            {
                if (Base3::Add(key3, value))
                {
                    if (Base4::Add(key4, value))
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
        Base3::Remove(key3);
    _rm2:
        Base2::Remove(key2);
    _rm1:
        Base1::Remove(key1);
        return false;
    }

    // 只能从单个容器中删除，如果需要同步删除则需要提供更多的Key
    bool RemoveByKey1(typename Base1::const_key_reference key)
    {
        return Base1::Remove(key);
    }

    bool RemoveByKey2(typename Base2::const_key_reference key)
    {
        return Base2::Remove(key);
    }

    bool RemoveByKey3(typename Base3::const_key_reference key)
    {
        return Base4::Remove(key);
    }

    bool RemoveByKey4(typename Base4::const_key_reference key)
    {
        return Base4::Remove(key);
    }

    int Remove(typename Base1::const_key_reference key1,
               typename Base2::const_key_reference key2 = typename Base2::key_type(),
               typename Base3::const_key_reference key3 = typename Base3::key_type(),
               typename Base4::const_key_reference key4 = typename Base4::key_type())
    {
        int c = Base1::Remove(key1);
        c += Base2::Remove(key2);
        c += Base3::Remove(key3);
        c += Base4::Remove(key4);
        return c;
    }

    inline const_value_reference FindByKey1(typename Base1::const_key_reference key1) const { return Base1::Find(key1); }
    inline const_value_reference FindByKey2(typename Base2::const_key_reference key2) const { return Base2::Find(key2); }
    inline const_value_reference FindByKey3(typename Base3::const_key_reference key3) const { return Base3::Find(key3); }
    inline const_value_reference FindByKey4(typename Base4::const_key_reference key4) const { return Base4::Find(key4); }
};

} // namespace XSpace

#endif // _MULTIINDEXCONTAINER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
