
#ifndef _UVPP_H_
#define _UVPP_H_

#include "NetAddress.h"
#include "UVAsync.h"
#include "UVBuf.h"
#include "UVCheck.h"
#include "UVHandle.h"
#include "UVIdle.h"
#include "UVLoop.h"
#include "UVPoll.h"
#include "UVPrepare.h"
#include "UVReq.h"
#include "UVReqConnect.h"
#include "UVReqShutdown.h"
#include "UVReqWrite.h"
#include "UVStream.h"
#include "UVTcp.h"
#include "UVTimer.h"
#include "UVTty.h"
#include "UVTypes.h"
#include "UVUdp.h"
#include "uv.h"

namespace XSpace
{

inline const char* Version()
{
    return uv_version_string();
}

} // namespace XSpace

#endif // _UVPP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
