
#ifndef _CORECONFIG_H_
#define _CORECONFIG_H_

namespace XSpace
{

#ifdef SLICE_MSGID_TYPE
typedef SLICE_MSGID_TYPE MsgID_t;
#else
typedef unsigned int MsgID_t;
#endif

class CoreConfig
{
public:

private:

};

} // namespace XSpace

#endif // _CORECONFIG_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

