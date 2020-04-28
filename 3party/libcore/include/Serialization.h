
#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include "Stream.h"
#include "Config.h"

namespace XSpace
{

class ISerialization
{
public:
    ISerialization() {}
    virtual ~ISerialization() {}
    virtual void Serialize(OStream &stream) const = 0;
    virtual void Unserialize(IStream &stream) = 0;
};

class Serializable : public ISerialization
{
public:
    Serializable() {}
    ~Serializable() {}
    virtual void Serialize(OStream &) const OVERRIDE {}
    virtual void Unserialize(IStream &) OVERRIDE {}
};

} // namespace XSpace

#endif // SERIALIZATION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
