
#ifndef UVTYPES_H_
#define UVTYPES_H_

namespace XNode
{

enum UVEventType
{
    UVET_READ = 1,
    UVET_WRITE = 2,
    UVET_DISCONNECT = 4,
    UVET_PRIORITIZED = 8,
    UVET_ALL
};

} // namespace XNode

#endif // UVTYPES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

