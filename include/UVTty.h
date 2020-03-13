
#ifndef _UVTTY_H_
#define _UVTTY_H_

#include "UVIODevice.h"

namespace XNode
{

class UVLoop;

class UVTty : public UVIODevice 
{
public:
    explicit UVTty(UVLoop *loop, int fd, bool readable = true, int flag = 0);
    ~UVTty();

    void OnRead(void *data, int nread);
    void OnClosed();
    void Release();
    void OnShutdown();
};

} // namespace XNode

#endif // _UVTTY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
