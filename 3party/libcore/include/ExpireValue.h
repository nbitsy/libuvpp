
#ifndef _EXPIREVALUE_H_
#define _EXPIREVALUE_H_

// for Timestamp
#include "Timestamp.h"
// for remove_const<T>
#include "TypeTraits.h"

namespace XSpace
{

template <typename V>
class ExpireValue
{
public:
    typedef typename remove_const<V>::type value_type;
    typedef value_type& value_reference;
    typedef const value_type& const_value_reference;

public:
    ExpireValue() {}
    ExpireValue(const_value_reference v, const Timestamp& expired)
        : _value(v), _expired(expired) {}
    ~ExpireValue() {}

    inline const_value_reference Get() const { return _value; }
    inline void Set(const_value_reference v) { _value = v; }

    inline bool IsExpired() const { return _expired <= Timestamp(); }
    inline void SetExpired(const Timestamp& t) { _expired = t; }

private:
    // 具体的值
    V _value;
    // 到期时间
    Timestamp _expired;
};

} // namespace XSpace

#endif // _EXPIREVALUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
