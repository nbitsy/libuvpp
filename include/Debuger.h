
#ifndef DEBUGER_H_
#define DEBUGER_H_

#include <stdlib.h>

namespace XNode
{

#define DEBUG_WHITEnBACK ""
#define DEBUG_GREENnBACK "\033[1;32;40m"
#define DEBUG_BLUEnBACK "\033[1;34;40m"
#define DEBUG_YALLOWnBACK "\033[1;33;40m"
#define DEBUG_REDnBACK "\033[1;31;40m"
#define DEBUG_WHITEnRED "\033[1;30;41m"
#define DEBUG_YALLOWnRED "\033[1;33;41m"
#define DEBUG_BACKnRED "\033[1;30;41m"

#define DEBUG_END "\033[0m"

#ifdef _DEBUG
#define DEBUG(...)                                                                      \
    {                                                                                   \
        fprintf(stderr, "%s%s=>%s ", DEBUG_GREENnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                 \
    }
#define WARN(...)                                                                        \
    {                                                                                    \
        fprintf(stderr, "%s%s=>%s ", DEBUG_YALLOWnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                  \
    }
#define ERROR(...)                                                                    \
    {                                                                                 \
        fprintf(stderr, "%s%s=>%s ", DEBUG_REDnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                               \
    }
#else
#define DEBUG(...)
#define WARN(...)
#define ERROR(...)
#endif

} // namespace XNode

#endif // DEBUGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
