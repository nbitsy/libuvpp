
#ifndef _RANDOM_H_
#define _RANDOM_H_

namespace XSpace
{

class Random
{
public:
    static int Rand();
    static int Rand(long end);
    // [star, end]
    static long Rand(long start, long end);

public:
    Random();
    Random(unsigned int seed);
    ~Random();

private:
    void Init(unsigned int seed);
    void UnInit();
    long RandVal();

private:
    unsigned int _seed;
#if defined(USE_RANDOM) && USE_RANDOM
    int _randomFd;
#elif defined(USE_URANDOM) && USE_URANDOM
    int _urandomFd;
#endif
};

} // namespace XSpace

#endif // _RANDOM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

