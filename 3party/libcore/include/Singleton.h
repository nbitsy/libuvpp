
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "Config.h"
#include "Noncopyable.h"
#include "TypeTraits.h"
#ifdef SINGLETON_SAFE
#include "Mutex.h"
#endif

// XXX: 这个单例有个问题，就是不同的动态库里会有不同的副本！！！！！！！！！！
// XXX: 如果需要全局单例请使用 GlobalSingleton ！！！！！！！！！！！！！!

// XXX: 对于thread_local的变量读速度比非thread_local的变量的读速度慢1/2
// XXX: 对于thread_local的变量读写速度比非thread_local的变量的读写速度慢1/3

namespace XSpace
{

// 单例
template <typename T>
class Singleton : public Noncopyable
{
public:
    Singleton() {}
    ~Singleton() {}

    template <typename U>
    inline static T &InstanceWith(U *u, bool force = false)
    {
        if (NULL == u)
            return *_instance; // XXX: Crash

        if (!is_subclass<U, T>::value)
            return *_instance; // XXX: Crash

        if (_instance != NULL && force)
        {
            delete _instance;
            _instance = u;
            return *_instance;
        }

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
    inline static T &InstanceWith(U *u, bool force = false)
    {
        if (NULL == u)
            return *_tlinstance; // XXX: Crash

        if (!is_subclass<U, T>::value)
            return *_tlinstance; // XXX: Crash

        if (_tlinstance != NULL && force)
        {
            delete _tlinstance;
            _tlinstance = u;
            return *_tlinstance;
        }

        _tlinstance = u;
        return *_tlinstance;
    }

    inline static T& Instance()
    {
        if (!_tlinstance)
            _tlinstance = new T();

        return *_tlinstance;
    }

protected:
    thread_local static T *_tlinstance;
};

template <typename T>
thread_local T *ThreadSingleton<T>::_tlinstance = 0;

} // namespace XSpace

#endif // _SINGLETON_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
