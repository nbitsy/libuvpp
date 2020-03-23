
#ifndef _XRAPIDJSON_H_
#define _XRAPIDJSON_H_

#include "Noncopyable.h"
#include "document.h"
#include <string>

namespace XSpace
{

typedef rapidjson::Value RapidJsonValue;

// get member
RapidJsonValue &operator&(RapidJsonValue &jvalue, const char *name);
// has member
bool operator&&(RapidJsonValue &jvalue, const char *name);

class RapidJson : public Noncopyable
{
public:
    static RapidJsonValue null;

public:
    RapidJson();
    ~RapidJson();

    inline rapidjson::Document::AllocatorType &GetAllocator() { return _doc.GetAllocator(); }

    void Clear();

    bool Parse(const char *content);
    bool ParseFile(const std::string &file);
    bool SaveToFile(const std::string &file, bool pretty = true);

    RapidJsonValue &AddObject(const char *name);
    RapidJsonValue &AddArray(const char *name);

    RapidJson &AddMember(const char *name, RapidJsonValue &value);
    template <typename T>
    RapidJson &AddMember(const char *name, T value);
    RapidJson &AddMember(const char *name, long value);
    RapidJson &AddMember(const char *name, unsigned long value);
    RapidJson &AddMember(const char *name, const char *value);
    RapidJson &AddMember(const char *name, const std::string &value);

    bool HasMember(const char *name) const;
    RapidJsonValue &operator[](const char *name);
    const RapidJsonValue &operator[](const char *name) const;
    // get member
    inline RapidJsonValue &operator&(const char *name) { return operator[](name); }
    inline const RapidJsonValue &operator&(const char *name) const { return operator[](name); }
    bool operator&&(const char *name);

    std::string ToString();

private:
    rapidjson::Document _doc;
};

template <typename T>
RapidJson &RapidJson::AddMember(const char *name, T value)
{
    if (NULL == name)
        return *this;

    RapidJsonValue jname(rapidjson::kStringType);
    jname.SetString(name, strlen(name));
    _doc.AddMember(jname, value, _doc.GetAllocator());
    return *this;
}

} // namespace XSpace

#endif // _XRAPIDJSON_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
