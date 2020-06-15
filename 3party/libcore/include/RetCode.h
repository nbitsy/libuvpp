
#ifndef _RETCODE_H_
#define _RETCODE_H_

#include <string>

namespace XSpace
{

class RetCode
{
public:
    explicit RetCode(int id = -1) : _code(id) {}
    RetCode(const RetCode& other) : _code(other._code) {}
    virtual ~RetCode() {}

    inline int Code() const { return _code; }

    virtual std::string ToString() const { return "unknow"; }
    virtual const char* ToStringUnsafe() const { return "unknow"; }

    inline RetCode& operator=(int id)
    {
        this->_code = id;
        return *this;
    }

private:
    int _code; // -1,unknow
};

} // namespace XSpace

#endif // _RETCODE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
