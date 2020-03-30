
#ifndef _UVTTY_H_
#define _UVTTY_H_

#include "UVIODevice.h"

namespace XSpace
{

class UVLoop;

class UVTty : public UVIODevice
{
public:
    UV_CREATE_HANDLE(UVTty)

public:
    explicit UVTty(const std::weak_ptr<UVLoop> &loop, int fd, bool readable = true, int flag = 0);
    ~UVTty();

    void OnRead(void *data, int nread) OVERRIDE;
    void OnClosed() OVERRIDE;
    void OnShutdown();
};

} // namespace XSpace

#endif // _UVTTY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
