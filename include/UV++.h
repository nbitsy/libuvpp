
#ifndef UV_PP_H_
#define UV_PP_H_

#include "uv.h"
#include "UVLoop.h"
#include "UVHandle.h"
#include "UVReq.h"
#include "UVReqShutdown.h"
#include "UVReqWrite.h"
#include "UVStream.h"
#include "UVTcp.h"
#include "UVUdp.h"


namespace XNode
{

inline const char* Version()
{
    return uv_version_string();
}

} // namespace XNode

#endif // UV_PP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

