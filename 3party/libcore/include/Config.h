
#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef __cplusplus
#define __EBEGIN \
    extern "C"   \
    {
#define __EEND }
#define __NBEGIN     \
    namespace XSpace \
    {
#define __NEND }
#else
#define __EBEGIN
#define __EEND
#define __NBEGIN
#define __NEND
#endif

// hardware macros
#if defined(__i386__) || defined(i386) || defined(_X86_) || defined(_M_IX86) || defined(_WIN32)
#define CPU_32BIT 1
#endif

#if defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_WIN64)
#define CPU_64BIT 1
#endif

#define SMP 1

#undef LITTLE_ENDIAN
#if defined(__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__
#define LITTLE_ENDIAN 1
#endif

#undef BIG_ENDIAN
#if defined(__BIG_ENDIAN__) && __BIG_ENDIAN__
#define BIG_ENDIAN 1
#endif

// OS
#define _OS_FREE_BSD 0x0001
#define _OS_LINUX 0x0005
#define _OS_MAC_OS_X 0x0006
#define _OS_NET_BSD 0x0007
#define _OS_OPEN_BSD 0x0008
#define _OS_CYGWIN 0x000d
#define _OS_UNKNOWN_UNIX 0x00ff
#define _OS_WINDOWS_NT 0x1001

#if defined(__FreeBSD__)
#define _OS_FAMILY_UNIX 1
#define _OS_FAMILY_BSD 1
#define _OS _OS_FREE_BSD
#define UNIXLIKE 1
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#define _OS_FAMILY_UNIX 1
#define _OS _OS_LINUX
#define UNIXLIKE 1
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define _OS_FAMILY_UNIX 1
#define _OS_FAMILY_BSD 1
#define _OS _OS_MAC_OS_X
#define UNIXLIKE 1
#elif defined(__NetBSD__)
#define _OS_FAMILY_UNIX 1
#define _OS_FAMILY_BSD 1
#define _OS _OS_NET_BSD
#define UNIXLIKE 1
#elif defined(__OpenBSD__)
#define _OS_FAMILY_UNIX 1
#define _OS_FAMILY_BSD 1
#define _OS _OS_OPEN_BSD
#define UNIXLIKE 1
#elif defined(unix) || defined(__unix) || defined(__unix__)
#define _OS_FAMILY_UNIX 1
#define _OS _OS_UNKNOWN_UNIX
#define UNIXLIKE 1
#elif defined(_WIN32) || defined(_WIN64)
#define _OS_FAMILY_WINDOWS 1
#define _OS _OS_WINDOWS_NT
#define WINDOWS 1
#elif defined(__CYGWIN__)
#define _OS_FAMILY_UNIX 1
#define _OS _OS_CYGWIN
#define UNIXLIKE 1
#endif

// compiler macros
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if defined(__GNUC__) || defined(__CYGWIN__)
#define __COMPILER_GCC 1
#ifdef __cplusplus
#define __COMPILER_CXX 1
#endif
#endif

#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#if _MSC_VER < 1400
#define __COMPILER_MSVC7_OR_LOWER 1
#elif _MSC_VER < 1600
#define __COMPILER_MSVC9_OR_LOWER 1
#endif
#endif

#if __COMPILER_GCC || __COMPILER_CXX
#include <stddef.h>
#else
#define __PRETTY_FUNCTION__ __func__
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 4 && __GNUC_MINOR__ >= 3))
#define CXX0X 1
#endif

#if defined(__GNUC__)
#include <limits.h>
#endif

#if __COMPILER_GCC
#define ALIGNOF(type) __alignof__(type)
#define ALIGNMENT(type, varname, alignment) type varname __attribute__((__aligned__(alignment)))
#elif __COMPILER_MSVC
#define ALIGNOF(type) __alignof(type)
#define ALIGNMENT(type, varname, alignment) __declspec(align(alignment)) type varname
#else
#error need alignment
#endif

#ifndef OVERRIDE
#define OVERRIDE
#endif

#if __COMPILER_GCC || __COMPILER_CXX
#define ATTR_NORETURN __attribute__((noreturn))
#define ATTR_PRINTF(F, V) __attribute__((format(printf, F, V)))
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR __attribute__((destructor))
#else
#define ATTR_NORETURN
#define ATTR_PRINTF(F, V)
#define CONSTRUCTOR
#define DESTRUCTOR
#endif

// unuse
#undef UNUSE
#define UNUSE(x) ((void)(x))

#define CLRERR() (errno = 0)

#ifdef CPU_64BIT
#define I64_FMT "l"
#else
#define I64_FMT "ll"
#endif

typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
#ifdef CPU_64BIT
typedef unsigned long u64_t;
#else
typedef unsigned long long u64_t;
#endif

typedef float f32_t;
typedef double f64_t;
typedef long double f128_t;

typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
#ifdef CPU_64BIT
typedef long s64_t;
#else
typedef long long s64_t;
#endif

typedef u16_t strlen_t;

#define VOLATILE volatile

#if defined(CPU_64BIT)
typedef u64_t ptr_t;
#elif defined(CPU_32BIT)
typedef u32_t ptr_t;
#endif

#ifndef _OUT
#define _OUT
#endif
#ifndef _IN
#define _IN
#endif

// 不要修改任何与这个值有关的数据，包括指针指向的内容
#ifndef _NOMODIFY
#define _NOMODIFY
#endif

#endif // CONFIG_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
