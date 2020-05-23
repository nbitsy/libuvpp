
#ifndef _HRREDISPOOL_H_
#define _HRREDISPOOL_H_

#include <unordered_map>

namespace XSpace
{

class HRConnection;

class HRRedisPool
{
public:
    HRRedisPool();
    ~HRRedisPool();

    HRConnection* Get(unsigned int);
    void Pus(HRConnection* c);

private:
    std::unordered_map<unsigned int, HRConnection*> _redis;
};

} // namespace XSpace

#endif // _HRREDISPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
