
#include "UVLoop.h"
#include "Allocator.h"

namespace XSpace
{

static std::shared_ptr<UVLoop> gDefaultLoop;

template <typename T>
std::weak_ptr<UVLoop> UVLoop::DefaultLoop()
{
    if (gDefaultLoop == NULL)
        gDefaultLoop = UVLoop::Create<T>("Default", true);
    return gDefaultLoop;
}

UVLoop::UVLoop(const std::string& name)
    : UVDataHelper(), _name(name), _loop(NULL), _running(false)
{
    uv_replace_allocator(Allocator::malloc, Allocator::realloc, Allocator::calloc, Allocator::free);

    _loop = (uv_loop_t*)Allocator::malloc(sizeof(*_loop));
    if (_loop != NULL)
    {
        uv_loop_init(_loop);
        uv_loop_set_data(_loop, NULL);
    }

    _threadId = std::this_thread::get_id();
    DEBUG("Object @%p\n", this);
}

UVLoop::~UVLoop()
{
    DEBUG("Object @%p\n", this);
    if (_loop != NULL)
    {
        ClearData();
        uv_loop_close(_loop);
        Allocator::free(_loop);
        _loop = NULL;
    }
}

void UVLoop::SetData(void* data, bool force, bool strong)
{
    UVDataHelper::SetData(this, _loop, data, force, strong);
}

UVData* UVLoop::GetData() const
{
    return UVDataHelper::GetData(_loop);
}

void UVLoop::ClearData()
{
    UVDataHelper::ClearData(_loop);
}

bool UVLoop::Run(uv_run_mode type)
{
    if (IsRunning())
        return true;

    if (_loop != NULL)
    {
        _running = true;
        INFO("Thread from %p to %p\n", FormatedThreadId(), FormatThreadId(std::this_thread::get_id()));
        _threadId = std::this_thread::get_id();
        uv_run(_loop, type);
        OnStopped();
        _running = false;
    }

    return _running;
}

bool UVLoop::Start()
{
    return Run(UV_RUN_DEFAULT);
}

bool UVLoop::StartOnce()
{
    return Run(UV_RUN_ONCE);
}

bool UVLoop::StartNowait()
{
    return Run(UV_RUN_NOWAIT);
}

void UVLoop::StopLoop()
{
    if (!IsRunning())
        return;

    if (_loop != NULL)
    {
        uv_stop(_loop);
        _running = false;
    }
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
        return uv_loop_alive(_loop);
    return false;
}

void UVLoop::OnStopped()
{
    DEBUG("\n");
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
