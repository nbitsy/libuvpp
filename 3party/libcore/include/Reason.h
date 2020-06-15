
#ifndef _REASON_H_
#define _REASON_H_

#include "RetCode.h"

namespace XSpace
{

class Reason : public RetCode
{
public:
    explicit Reason(int code = -1) : RetCode(code) {}
    virtual ~Reason() {}

    std::string ToString() const { return "unknow reason"; }
    const char* ToStringUnsafe() const { return "unknow reason"; }

    inline Reason& operator=(int code)
    {
        (RetCode&)(*this) = code;
        return *this;
    }
};

inline std::string ToString(const Reason& result) { return result.ToString(); }
inline const char* ToStringSafe(const Reason& result) { return result.ToStringUnsafe(); }

} // namespace XSpace

#endif // _REASON_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
