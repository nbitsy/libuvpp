
#ifndef _FASTSYNCDEQUE_H_
#define _FASTSYNCDEQUE_H_

#include "SyncDeque.h"

namespace XSpace
{

// TODO: 基于spinlock的互斥同步deque
template <typename T>
class FastSyncDeque : public SyncDeque<T>
{
public:
    FastSyncDeque() {}
    ~FastSyncDeque() {}

private:
};

} // namespace XSpace

#endif // _FASTSYNCDEQUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

