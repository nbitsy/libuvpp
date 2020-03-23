
#ifndef _RPCSERVICE_H_
#define _RPCSERVICE_H_

#include "Debuger.h"
#include "Serialization.h"
#include "Stream.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace XSpace
{

class RPCCall : public ISerialization
{
public:
    RPCCall() {}
    ~RPCCall() {}

    virtual void Serialize(OStream &stream) const = 0;
    virtual void Unserialize(IStream &stream) = 0;
    virtual void OnCall(void *) = 0;
};

class RPCService
{
public:
    RPCService() {}
    virtual ~RPCService();
    virtual bool Init();
    bool OnCall(IOStream &stream);
private:
    std::unordered_map<unsigned int, RPCCall *> _calls;
};

} // namespace XSpace

#endif // _RPCSERVICE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
