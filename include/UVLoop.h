
#ifndef _UVLOOP_H_
#define _UVLOOP_H_

#include <memory>
#include <string>
#include <thread>

#include "Allocator.h"
#include "Config.h"
#include "TypeTraits.h"
#include "UVDataHelper.h"
#include "UVPoolHelper.h"

#define UV_CREATE_LOOP(TYPE)                                                                    \
    template <typename T = TYPE>                                                                \
    static std::shared_ptr<TYPE> Create(const std::string &name, bool useDefault = false)       \
    {                                                                                           \
        return CreateWeak<T>(name, useDefault);                                                 \
    }                                                                                           \
    template <typename T = TYPE>                                                                \
    static std::shared_ptr<TYPE> CreateWeak(const std::string &name, bool useDefault = false)   \
    {                                                                                           \
        if (is_subclass<TYPE, UVLoop>::value)                                                   \
        {                                                                                       \
            std::shared_ptr<TYPE> ptr(new TYPE(name));                                          \
            if (ptr != NULL)                                                                    \
                ptr->SetData(NULL, true, false);                                                \
                                                                                                \
            return ptr;                                                                         \
        }                                                                                       \
                                                                                                \
        return NULL;                                                                            \
    }                                                                                           \
    template <typename T = TYPE>                                                                \
    static std::shared_ptr<TYPE> CreateShared(const std::string &name, bool useDefault = false) \
    {                                                                                           \
        if (is_subclass<TYPE, UVLoop>::value)                                                   \
        {                                                                                       \
            std::shared_ptr<TYPE> ptr(new TYPE(name));                                          \
            if (ptr != NULL)                                                                    \
                ptr->SetData(NULL, true, true);                                                 \
                                                                                                \
            return ptr;                                                                         \
        }                                                                                       \
                                                                                                \
        return NULL;                                                                            \
    }

namespace XSpace
{

class UVLoop : public UVDataHelper, public std::enable_shared_from_this<UVLoop>
{
public:
    UV_CREATE_LOOP(UVLoop)
    DRIVED_FROM_THIS(UVLoop);

public:
    // XXX: 对于DefaultLoop来说，是第一个调用DefaultLoop传入的T
    template <typename T>
    static std::weak_ptr<UVLoop> DefaultLoop();

public:
    virtual ~UVLoop();

    void SetData(void *data, bool force = false, bool strong = false);
    UVData *GetData() const;
    void ClearData();

    bool Start();
    bool StartOnce();
    bool StartNowait();
    void StopLoop();
    inline void Stop() { StopLoop(); }
    virtual void OnStopped();

    int RawFd() const;
    bool IsAlive() const;
    inline const std::string &Name() const { return _name; }

    std::weak_ptr<UVLoop> GetLoop() { return shared_from_this(); }

    template <typename T>
    T *GetRawLoop() { return reinterpret_cast<T *>(_loop); }

    inline std::thread::id ThreadId() const { return _threadId; }
    inline bool IsRunning() const { return _running; }

private:
    bool Run(uv_run_mode type);

protected:
    explicit UVLoop(const std::string &name);

protected:
    uv_loop_t *_loop;
    std::string _name;
    std::thread::id _threadId;
    bool _running;
};

} // namespace XSpace

#endif // _UVLOOP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
