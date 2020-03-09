
#ifndef UVPOLL_H_
#define UVPOLL_H_

#include "UVHandle.h"
#include "UVTypes.h"

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

    virtual void OnClosed();
    virtual void OnPoll(int status, int event);
};

} // namespace XNode

#endif // UVPOLL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
