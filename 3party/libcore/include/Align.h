
#ifndef _ALIGN_H_
#define _ALIGN_H_

#include <memory>

// align必须是2^n，否则结果不可预测
inline size_t Align(size_t size, size_t align)
{
    return (size + (align - 1) & ~(align - 1));
}

#ifndef PAGE_SZIE
#define PAGE_SIZE (4*1024)
#endif

#define ALIGN(size, align) ((size) + ((align) - 1) & ~((align) - 1))
#define PAGE_ALIGN(size) ALIGN(size, PAGE_SIZE)

#endif // _ALIGN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
