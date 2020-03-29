
#ifndef _UVPOLL_H_
#define _UVPOLL_H_

#include "UVTypes.h"
#include "UVHandle.h"

namespace XSpace
{

class UVLoop;

class UVPoll : public UVHandle
{
public:
    UV_CREATE_HANDLE(UVPoll)

public:
    UVPoll(std::weak_ptr<UVLoop>& loop, int fd);
    ~UVPoll();

    bool Start(int events = UVET_ALL);
    bool Stop();

    void OnClosed() OVERRIDE;

    virtual void OnPoll(int status, int event);
};

} // namespace XSpace

#endif // _UVPOLL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
