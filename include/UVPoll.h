
#ifndef _UVPOLL_H_
#define _UVPOLL_H_

#include "UVTypes.h"
#include "UVHandle.h"

namespace XNode
{

class UVLoop;

class UVPoll : public UVHandle
{
public:
    UVPoll(UVLoop *loop, int fd);
    ~UVPoll();

    bool Start(int events = UVET_ALL);
    bool Stop();

    void OnClosed();
    virtual void OnPoll(int status, int event);
    void Release();
};

} // namespace XNode

#endif // _UVPOLL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
