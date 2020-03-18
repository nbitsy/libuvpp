
#ifndef _DEBUGER_H_
#define _DEBUGER_H_

#ifdef _DEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

namespace XNode
{

#ifdef _DEBUG

#define DEBUG_GREENnBACK "\033[1;32;40m"  // debug
#define DEBUG_WHITEnBACK ""               // log
#define DEBUG_BLUEnBACK "\033[1;34;40m"   // info
#define DEBUG_YALLOWnBACK "\033[1;33;40m" // warn
#define DEBUG_REDnBACK "\033[1;31;40m"    // err
#define DEBUG_WHITEnRED "\033[1;30;41m"
#define DEBUG_YALLOWnRED "\033[1;33;41m"
#define DEBUG_BACKnRED "\033[1;30;41m"

#define DEBUG_END "\033[0m"

#define DEBUG_LEVEL_DEBUG 0
#define DEBUG_LEVEL_LOG 1
#define DEBUG_LEVEL_INFO 2
#define DEBUG_LEVEL_WARN 3
#define DEBUG_LEVEL_ERR 4

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_LEVEL_DEBUG
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_DEBUG
#define DEBUG(...)                                                                     \
    {                                                                                  \
        fprintf(stderr, "%s%s %s ", DEBUG_GREENnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                \
    }
#else
#define DEBUG(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_LOG
#define LOG(...)                                                         \
    {                                                                    \
        fprintf(stderr, "%s%s ", DEBUG_WHITEnBACK, __PRETTY_FUNCTION__); \
        fprintf(stdout, ##__VA_ARGS__);                                  \
    }
#else
#define LOG(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
#define INFO(...)                                                                    \
    {                                                                                \
        fprintf(stderr, "%s%s %s", DEBUG_BLUEnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stdout, ##__VA_ARGS__);                                              \
    }
#else
#define INFO(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_WARN
#define WARN(...)                                                                       \
    {                                                                                   \
        fprintf(stderr, "%s%s %s ", DEBUG_YALLOWnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                 \
    }
#else
#define WARN(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_WARN
#define ERROR(...)                                                                   \
    {                                                                                \
        fprintf(stderr, "%s%s %s ", DEBUG_REDnBACK, __PRETTY_FUNCTION__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                              \
    }
#else
#define ERROR(...)
#endif

#else
#define DEBUG(...)
#define LOG(...)
#define INFO(...)
#define WARN(...)
#define ERROR(...)
#endif

} // namespace XNode

#endif // _DEBUGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
