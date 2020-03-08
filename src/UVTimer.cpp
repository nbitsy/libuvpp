
#include "UVTimer.h"

namespace XNode
{

UVTimer::UVTimer(UVLoop *loop) : UVHandle(loop)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_timer_t)); // TODO:
    if (_handle != NULL && _loop != NULL && _loop->GetLoop<uv_loop_t>() != NULL)
    {
        uv_handle_set_data(_handle, NULL);
        uv_timer_init(_loop->GetLoop<uv_loop_t>(), GetHandle<uv_timer_t>());
        SetData(&_timestamp);
    }
    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVTimer::~UVTimer()
{
    if (_handle != NULL)
        Stop();

    std::cout << "Object@"<< (void*)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

void __OnTimer(uv_timer_t *timer)
{
    UVData *data = (UVData *)uv_handle_get_data((uv_handle_t*)timer);
    if (NULL == data)
        return;

    UVTimer *uvtimer = (UVTimer *)data->_self;
    if (NULL == uvtimer)
        return;
    
    Timestamp* timestamp = (Timestamp*)data->_data;
    if (timestamp != NULL)
        timestamp->Update();

    uvtimer->Tick(timestamp);
}

// timeout: 第一次开始时的延迟时间
// repeat: 下一次的回调的间隔时间,ms
bool UVTimer::Start(uint64_t repeat, uint64_t timeout)
{
    if (NULL == _handle || !_handle->flags)
        return false;

    return !uv_timer_start(GetHandle<uv_timer_t>(), __OnTimer, timeout, repeat);
}

void UVTimer::Stop()
{
    if (NULL == _handle || !_handle->flags)
        return;

    uv_timer_stop(GetHandle<uv_timer_t>());
    Close();
}

void UVTimer::SetRepeat(unsigned long long repeat)
{
    if (NULL == _handle)
        return;

    uv_timer_set_repeat(GetHandle<uv_timer_t>(), repeat);
}

unsigned long long UVTimer::GetRepeat() const
{
    if (NULL == _handle)
        return 0;

    return uv_timer_get_repeat(GetHandle<uv_timer_t>());
}

void UVTimer::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    delete this; // TODO:
}

void UVTimer::Tick(Timestamp* now)
{
    if (now != NULL)
        std::cout << __PRETTY_FUNCTION__ << " Now: " << now->Ticks() << std::endl;
    else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
