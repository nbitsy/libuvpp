
#ifndef _UVLOOP_H_
#define _UVLOOP_H_

#include "UVDataHelper.h"
#include "UVPoolHelper.h"
#include <string>
#include <thread>

namespace XNode
{

class UVLoop : public UVDataHelper, public UVPoolHelper
{
public:
    explicit UVLoop(const std::string &name, bool useDefault = false);
    virtual ~UVLoop();

    void SetData(void *data, bool force = false);
    UVData *GetData() const;
    void ClearData();

    bool Start();
    bool StartOnce();
    bool StartNowait();
    void StopLoop();
    inline void Stop() { StopLoop(); }

    int RawFd() const;
    bool IsAlive() const;
    inline const std::string& Name() const { return _name; }

    UVLoop *GetLoop() { return this; }
    void Release() {/*do nothing*/}
    static UVLoop *DefaultLoop();

    template <typename T>
    T *GetRawLoop() { return reinterpret_cast<T *>(_loop); }

    inline std::thread::id ThreadId() const { return _threadId; }
    inline bool IsRunning() const { return _running; }

private:
    bool Run(uv_run_mode type);

protected:
    uv_loop_t *_loop;
    std::string _name;
    std::thread::id _threadId;
    bool _running;
};

} // namespace XNode

#endif // _UVLOOP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
