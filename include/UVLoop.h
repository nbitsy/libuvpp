
#ifndef UVLOOP_H_
#define UVLOOP_H_

#include "UVDataHelper.h"
#include <string>
#include <thread>

namespace XNode
{

class UVLoop : public UVDataHelper
{
public:
    explicit UVLoop(const std::string &name, bool useDefault = false);
    virtual ~UVLoop();

    void SetData(void *data, bool force = false);
    UVData *GetData() const;
    void ClearData();

    void Run();
    void RunOnce();
    void RunNowait();
    void StopLoop();

    int RawFd() const;
    bool IsAlive() const;

    static UVLoop *DefaultLoop();

    template <typename T>
    T *GetLoop() { return reinterpret_cast<T *>(_loop); }

    std::thread::id ThreadId() const { return _threadId; }

protected:
    uv_loop_t *_loop;
    std::string _name;
    std::thread::id _threadId;
};

} // namespace XNode

#endif // UVLOOP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
