
#ifndef _CONFIGURATIONSLUA_H_
#define _CONFIGURATIONSLUA_H_

#include <string>

#include "Config.h"
#include "Tools/LuaTinker.h"

namespace XSpace
{

typedef LuaTable ConfigurationValueLua;

class ConfigurationsLua : public LuaTinker
{
public:
    explicit ConfigurationsLua(const std::string& path, const std::string& section = "");
    virtual ~ConfigurationsLua();

    virtual bool Init(const std::string& path = "") OVERRIDE;
    inline const std::string& GetSection() const { return _section; }

    template <typename T>
    T Get(const char* name)
    {
        static T t = T();
        if (_config)
            return _config->get<T>(name);

        return t;
    }

private:
    std::string _confpath;
    std::string _section;
    LuaTable* _config;
};

} // namespace XSpace

#endif // _CONFIGURATIONSLUA_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

