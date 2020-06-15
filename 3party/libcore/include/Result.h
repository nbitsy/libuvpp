
#ifndef _RESULT_H_
#define _RESULT_H_

#include "RetCode.h"

namespace XSpace
{

class Result : public RetCode
{
public:
    explicit Result(int code = -1) : RetCode(code) {}
    virtual ~Result() {}

    std::string ToString() const { return "unknow result"; }
    const char* ToStringUnsafe() const { return "unknow result"; }

    inline Result& operator=(int code)
    {
        (RetCode&)(*this) = code;
        return *this;
    }
};

inline std::string ToString(const Result& result) { return result.ToString(); }
inline const char* ToStringSafe(const Result& result) { return result.ToStringUnsafe(); }

} // namespace XSpace

#endif // _RESULT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
