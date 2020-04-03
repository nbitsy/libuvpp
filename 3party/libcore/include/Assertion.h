
#ifndef _ASSERTION_H_
#define _ASSERTION_H_

#include "Debugger.h"

#include <stdlib.h> // for abort

#ifdef _DEBUG

#define CRASH()                   \
    do                            \
    {                             \
        ERROR("!!!Crashed!!!\n"); \
        abort();                  \
    } while (0);

#define ASSERT(assertion)      \
    do                         \
    {                          \
        if (!(assertion))      \
        {                      \
            ERROR(#assertion); \
            CRASH();           \
        }                      \
    } while (0);

#else // !_DEBUG

#define CRASH() (void)0;
#define ASSERT(assertion) (void)0;

#endif // _DEBUG

#endif // _ASSERTION_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
