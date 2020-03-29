
#include "UVTty.h"
#include "UVLoop.h"

namespace XSpace
{

UVTty::UVTty(std::weak_ptr<UVLoop>& loop, int fd, bool readable, int flags) : UVIODevice(loop, flags)
{
    _handle = (uv_handle_t *)Allocator::malloc(sizeof(uv_tty_t));
    if (_handle != NULL && !loop.expired())
    {
        uv_tty_init(loop.lock()->GetRawLoop<uv_loop_t>(), (uv_tty_t *)_handle, fd, readable);
        uv_tty_set_mode((uv_tty_t*)_handle, 0);
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVTty::~UVTty()
{
    DEBUG("Object @%p\n", this);
}

void UVTty::OnRead(void *data, int nread)
{
    ((char*)data)[nread-1] = '\0';
    DEBUG("Read from TTY: %s(%d)\n", data, nread);
}

void UVTty::OnClosed()
{
    DEBUG("\n");
}

void UVTty::OnShutdown()
{
    DEBUG("\n");
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
