
#ifndef _SYNCDEQUE_H_
#define _SYNCDEQUE_H_

#include <deque>
#include <mutex>

namespace XSpace
{

template <typename T>
class SyncDeque
{
public:
    typedef T value_type;
    typedef std::deque<T> container_type;
    typedef typename container_type::iterator iterator_type;
    typedef std::recursive_mutex mutex_type;

public:
    SyncDeque() {}
    ~SyncDeque() {}

    inline void PushBack(T &v)
    {
        std::lock_guard<mutex_type> l(_lck);
        _queue.push_back(v);
    }

    T &PopBack()
    {
        std::lock_guard<mutex_type> l(_lck);
        T &v = _queue.back();
        _queue.pop_back();
        return v;
    }

    void PushFront(T &v)
    {
        std::lock_guard<mutex_type> l(_lck);
        _queue.push_front(v);
    }

    T &PopFront()
    {
        std::lock_guard<mutex_type> l(_lck);
        T &v = _queue.front();
        _queue.pop_front();
        return v;
    }

    inline void Swap(SyncDeque& other)
    {
        std::lock_guard<mutex_type> l(_lck);
        other._queue.swap(_queue);
    }

    template <typename F>
    void ForEach(const F& f)
    {
        std::lock_guard<mutex_type> l(_lck);
        for (auto i = _queue.begin(); i != _queue.end(); ++i)
            f(*i);
    }

    size_t Size() const
    {
        std::lock_guard<mutex_type> l(_lck);
        return _queue.size();
    }

private:
    container_type _queue;
    mutable mutex_type _lck;
};

} // namespace XSpace

#endif // _SYNCDEQUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
