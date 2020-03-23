
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "Config.h"
#include "Noncopyable.h"
#ifdef SINGLETON_SAFE
#include "Mutex.h"
#endif

namespace XSpace
{

template <typename T>
class Singleton : public Noncopyable
{
public:
    Singleton() {}
    ~Singleton() {}

    inline static T& Instance()
    {
        if (!_instance)
        {
#ifdef SINGLETON_SAFE
            ScopeLock lk(_lck);
            if (!_instance)
#endif
                _instance = new T();
        }
        return *_instance;
    }

protected:
    static T* _instance;
#ifdef SINGLETON_SAFE
    static Mutex _lck;
#endif
};

//template <typename T> T* Singleton<T>::_instance = BUFNEW T();
template <typename T> T* Singleton<T>::_instance = 0;
#ifdef SINGLETON_SAFE
template <typename T> Mutex Singleton<T>::_lck;
#endif


} // namespace XSpace

#endif // _SINGLETON_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

