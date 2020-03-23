
#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_

#include "Config.h"
#include "Thread.h"
#include <vector>

namespace XSpace
{

class ThreadGroup
{
public:
    typedef std::vector<Thread *> container;
    typedef container::size_type size_type;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    ThreadGroup(const std::string &name, const size_type maxsize = Thread::Concurrency())
        : _size(0), _max(maxsize), _name(name), _lock(), _container(_max * 2, NULL)
    {
    }

    virtual ~ThreadGroup()
    {
        std::lock_guard<std::mutex> l(_lock);
        for (size_type i = 0; i < _size; ++i)
        {
            delete (_container[i]); // TODO:
        }
    }

    bool Put(const Thread *thread);
    const Thread *Get(std::thread::id id) const;

    inline const std::string &getName() const { return _name; }
    const Thread *operator[](size_type index) const;
    Thread *operator[](size_type index);

    inline iterator Begin() { return _container.begin(); }
    inline iterator End() { return _container.end(); }
    inline const_iterator Begin() const { return _container.begin(); }
    inline const_iterator End() const { return _container.end(); }
    bool PushBack(const Thread *thread) { return Put(thread); }

    bool Erase(const Thread *thread);
    bool Erase(const char *threadname);
    bool Erase(const std::thread::id id);

    inline bool Erase(const std::string &threadname) { return Erase(threadname.c_str()); }

    virtual void StartAll();
    virtual void JoinAll();
    virtual void StopAll();

    inline size_type Size() const
    {
        std::lock_guard<std::mutex> l(_lock);
        return _size;
    }

    inline size_type MaxSize() const { return _max; }

protected:
    size_type _size;
    size_type _max;
    std::string _name;
    mutable std::mutex _lock;
    container _container;
    // std::map<std::thread::id, Thread*> _idMap;
};

} // namespace XSpace

#endif // _THREADGROUP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
