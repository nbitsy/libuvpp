
#ifndef _CONFIGURATIONS_H_
#define _CONFIGURATIONS_H_

#include <string>
#include <map>
#include "tools/XRapidJson.h"

namespace XSpace
{

typedef RapidJsonValue ConfigurationValue;

class Configurations : private RapidJson
{
public:
    static ConfigurationValue null;

public:
    explicit Configurations(const std::string& path, const std::string& section = "");
    virtual ~Configurations();

    virtual bool Load(const std::string& path = "");

    virtual const ConfigurationValue& Get(const std::string& key) const;
    virtual void Set(const std::string& key, ConfigurationValue& value);

    inline const std::string& GetSection() const { return _section; }

private:
    std::string _confpath;
    std::string _section;
};

} // namespace XSpace

#endif // _CONFIGURATIONS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

