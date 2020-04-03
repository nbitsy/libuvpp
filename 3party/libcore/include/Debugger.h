
#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#ifdef _DEBUG
#include "DateTime.h"
#include <stdio.h>
#include <stdlib.h>
using XSpace::DateTime;
#endif

namespace XSpace
{

#ifdef _DEBUG

#ifndef DEBUG_LEVEL
//#define DEBUG_LEVEL DEBUG_LEVEL_DEBUG
#define DEBUG_LEVEL DEBUG_LEVEL_LOG
#endif

#define ___FUNCTION___ __PRETTY_FUNCTION__
//#define ___FUNCTION___ __FUNCTION__
#define ___TIME___ DateTime().toString().c_str()

// 30 white 31 red 32 green 33 yellow 34 blue 35 purple 
#define DEBUG_GREENnBACK "\033[1;32;40m"  // debug
#define DEBUG_WHITEnBACK ""               // log
#define DEBUG_PURPLEnBACK "\033[1;35;40m" // info
#define DEBUG_YELLOWnBACK "\033[1;33;40m" // warn
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

#if DEBUG_LEVEL <= DEBUG_LEVEL_DEBUG
#define DEBUG(...)                                                                                           \
    {                                                                                                        \
        fprintf(stderr, "%s%s %s:%d%s ", DEBUG_GREENnBACK, ___TIME___, ___FUNCTION___, __LINE__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                                      \
    }
#else
#define DEBUG(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_LOG
#define LOG(...)                                                                                \
    {                                                                                           \
        fprintf(stderr, "%s%s %s:%d ", DEBUG_WHITEnBACK, ___TIME___, ___FUNCTION___, __LINE__); \
        fprintf(stdout, ##__VA_ARGS__);                                                         \
    }
#else
#define LOG(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
#define INFO(...)                                                                                             \
    {                                                                                                         \
        fprintf(stderr, "%s%s %s:%d %s", DEBUG_PURPLEnBACK, ___TIME___, ___FUNCTION___, __LINE__, DEBUG_END); \
        fprintf(stdout, ##__VA_ARGS__);                                                                       \
    }
#else
#define INFO(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_WARN
#define WARN(...)                                                                                              \
    {                                                                                                          \
        fprintf(stderr, "%s%s %s:%d %s ", DEBUG_YELLOWnBACK, ___TIME___, ___FUNCTION___, __LINE__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                                        \
    }
#else
#define WARN(...)
#endif

#if DEBUG_LEVEL <= DEBUG_LEVEL_WARN
#define ERROR(...)                                                                                               \
    {                                                                                                            \
        fprintf(stderr, "%s%s %s:%d %s ", DEBUG_REDnBACK, ___TIME___, __PRETTY_FUNCTION__, __LINE__, DEBUG_END); \
        fprintf(stderr, ##__VA_ARGS__);                                                                          \
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

} // namespace XSpace

#endif // _DEBUGGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
