
#include "UVLoop.h"
#include "Allocator.h"

namespace XNode
{

static UVLoop gDefaultLoop("Default", true);

UVLoop *UVLoop::DefaultLoop()
{
    return &gDefaultLoop;
}

UVLoop::UVLoop(const std::string &name, bool useDefault) : UVDataHelper(), UVPoolHelper(), _name(name), _loop(NULL)
{
    uv_replace_allocator(Allocator::malloc, Allocator::realloc, Allocator::calloc, Allocator::free);

    if (useDefault)
    {
        _loop = uv_default_loop();
        SetGC(false);
    }
    else
    {
        _loop = (uv_loop_t *)Allocator::malloc(sizeof(*_loop));
    }

    if (_loop != NULL)
    {
        uv_loop_init(_loop);
        uv_loop_set_data(_loop, NULL);
        SetData(NULL, true); // set this
    }

    _threadId = std::this_thread::get_id();
    DEBUG("Object @%p\n", this);
}

UVLoop::~UVLoop()
{
    if (_loop != NULL)
    {
        ClearData();

        if (GetGC())
        {
            uv_loop_close(_loop);
            Allocator::free(_loop);
        }
        else
        {
            uv_loop_close(_loop);
        }

        _loop = NULL;
    }
    DEBUG("Object @%p\n", this);
}

void UVLoop::SetData(void *data, bool force)
{
    UVDataHelper::SetData(_loop, data, force);
}

UVData *UVLoop::GetData() const
{
    return UVDataHelper::GetData(_loop);
}

void UVLoop::ClearData()
{
    UVDataHelper::ClearData(_loop);
}

void UVLoop::Start()
{
    if (_loop != NULL)
        uv_run(_loop, UV_RUN_DEFAULT);
}

void UVLoop::StartOnce()
{
    if (_loop != NULL)
        uv_run(_loop, UV_RUN_ONCE);
}

void UVLoop::StartNowait()
{
    if (_loop != NULL)
        uv_run(_loop, UV_RUN_NOWAIT);
}

void UVLoop::StopLoop()
{
    if (_loop != NULL)
        uv_stop(_loop);
}

int UVLoop::RawFd() const
{
    if (_loop != NULL)
        return uv_backend_fd(_loop);
    return -1;
}

bool UVLoop::IsAlive() const
{
    if (_loop != NULL)
        uv_loop_alive(_loop);
    return false;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
