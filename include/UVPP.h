
#ifndef UV_PP_H_
#define UV_PP_H_

#include "uv.h"
#include "EndPointAddress.h"
#include "UVTypes.h"
#include "UVLoop.h"
#include "UVAsync.h"
#include "UVBuf.h"
#include "UVCheck.h"
#include "UVHandle.h"
#include "UVIdle.h"
#include "UVPoll.h"
#include "UVPrepare.h"
#include "UVReq.h"
#include "UVReqConnect.h"
#include "UVReqShutdown.h"
#include "UVReqWrite.h"
#include "UVStream.h"
#include "UVTcp.h"
#include "UVTimer.h"
#include "UVUdp.h"
#include "UVTty.h"

namespace XNode
{

inline const char* Version()
{
    return uv_version_string();
}

} // namespace XNode

#endif // UV_PP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
