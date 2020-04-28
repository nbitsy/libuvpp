
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "Config.h"
#include "Noncopyable.h"
#include "TypeTraits.h"
#ifdef SINGLETON_SAFE
#include "Mutex.h"
#endif

namespace XSpace
{

// 全局单例
template <typename T>
class Singleton : public Noncopyable
{
public:
    Singleton() {}
    ~Singleton() {}

    template <typename U>
    inline static T &InstanceWith(U *u)
    {
        if (NULL == u)
            return *_instance; // XXX: Crash

        if (_instance != NULL)
            return *_instance;

        if (!is_subclass<U, T>::value)
            return *_instance; // XXX: Crash

        _instance = u;
        return *_instance;
    }

    inline static T &Instance()
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
    static T *_instance;
#ifdef SINGLETON_SAFE
    static Mutex _lck;
#endif
};

template <typename T>
T *Singleton<T>::_instance = 0;
#ifdef SINGLETON_SAFE
template <typename T>
Mutex Singleton<T>::_lck;
#endif

// 线程私有的单例
template <typename T>
class ThreadSingleton : public Noncopyable
{
public:
    ThreadSingleton() {}
    ~ThreadSingleton() {}

    template <typename U>
    inline static T &InstanceWith(U *u)
    {
        if (NULL == u)
            return *_instance; // XXX: Crash

        if (_instance != NULL)
            return *_instance;

        if (!is_subclass<U, T>::value)
            return *_instance; // XXX: Crash

        _instance = u;
        return *_instance;
    }

    inline static T &Instance()
    {
        if (!_instance)
        {
            if (!_instance)
                _instance = new T();
        }

        return *_instance;
    }

protected:
    thread_local static T *_instance;
};

template <typename T>
thread_local T *ThreadSingleton<T>::_instance = 0;

} // namespace XSpace

#endif // _SINGLETON_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
