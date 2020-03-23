
#ifndef _THREAD_H_
#define _THREAD_H_

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

namespace XSpace
{

class Thread
{
public:
    static void NSleep(long long nsecs);
    static void Sleep(long long msecs) { NSleep(msecs * 1000 * 1000L); }
    static void SSleep(long long secs) { Sleep(secs * 1000L); }

public:
    Thread(const std::string& name = "Thread")
        : _final(false), _name(name), _thread(NULL)
    {
    }

    ~Thread()
    {
        if (_thread != NULL && _thread->joinable())
        {
            delete _thread;
            _thread = NULL;
        }
    }

    inline const std::string &GetName() const { return _name; }
    inline std::thread::id Id() const { return _id; }
    inline std::thread::id ThreadId() const { return _id; }

    void Deatch();
    bool Joinable();
    void Join();

    inline void Terminate() { _final = true; }
    inline void Final() { _final = true; }
    inline bool IsAlive() { return _final == false; }

    bool Start();
    template <class Function, class... Args>
    bool Start(Function &&f, Args &&... args)
    {
        _thread = new std::thread(f, args...);
        return _thread != NULL;
    }

    virtual void Run();
    virtual void OnBeforeRun() {}
    virtual void OnEnd() {}

    void BeforeRun();
    void AfterRun();

    static int Concurrency() { return std::thread::hardware_concurrency(); }
    static std::thread::id CurrentThreadId() { return std::this_thread::get_id(); }

private:
    void StartRun(Thread *self);

private:
    volatile bool _final;
    std::thread::id _id;
    std::string _name;
    std::thread *_thread;
};

} // namespace XSpace

#endif // _THREAD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
