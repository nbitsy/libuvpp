
#ifndef _LUATINKER_H_
#define _LUATINKER_H_

/**
 * LuaTinker基于lua_tinker的一个简易C++接口，而至于底层用的是Lua还是LuaJIT并不关心
 * 但是，对于游戏服务器开发来说，还是建议使用LuaJIT。就目前来说LuaJIT只支持到Lua5.1，
 * 而lua_tinker也是对Lua5.1的支持，如果需要支持更高Lua版本，需要修改lua_tinker。
*/

extern "C"
{
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "Tools/lua_tinker.h"
#include <string>

using namespace lua_tinker;

namespace XSpace
{

typedef lua_tinker::table LuaTable;
typedef lua_State LuaState;

class LuaTinker
{
public:
    LuaTinker(const std::string &name);
    virtual ~LuaTinker();

    virtual bool Init(const std::string &file) { return true; }
    inline const std::string &GetName() const { return _name; }

    inline bool DoFile(const char *filename)
    {
        if (_L)
            return dofile(_L, filename);
        return false;
    }

    inline bool DoString(const char *str)
    {
        if (_L)
            return dostring(_L, str);
        return false;
    }

    inline bool DoBuffer(const char *buf, size_t size)
    {
        if (_L)
            return dobuffer(_L, buf, size);
        return false;
    }

    inline bool DoFile(const std::string &filename) { return DoFile(filename.c_str()); }
    inline bool DoString(const std::string &str) { return DoString(str.c_str()); }
    inline bool DoBuffer(const std::string &buf) { return DoBuffer(buf.c_str(), buf.length()); }
    inline void PushLightData(void *p) { lua_pushlightuserdata(_L, p); }

    template <typename T, typename... U>
    inline T Call(const char *name, U... args) { return lua_tinker::call<T>(_L, name, args...); }
    template <typename T>
    inline void SetGlobal(const char *name, T object) { lua_tinker::setglobal(_L, name, object); }

    template <typename T>
    inline T GetGlobal(const char *name) const { return lua_tinker::getglobal<T>(_L, name); }

    // global or static functions
    template <typename F>
    inline LuaTinker &Def(const char *name, F func)
    {
        lua_tinker::def(_L, name, func);
        return *this;
    }

    template <typename F>
    inline LuaTinker &DefFunc(const char *name, F func)
    {
        lua_tinker::def(_L, name, func);
        return *this;
    }

    // For class
    template <typename T>
    inline LuaTinker &Class(const char *name)
    {
        lua_tinker::class_add<T>(_L, name);
        return *this;
    }

    template <typename T, typename B>
    inline LuaTinker &ClassDrived(const char * /* name*/)
    {
        lua_tinker::class_inh<T, B>(_L);
        return *this;
    }

    template <typename T, typename... U>
    inline LuaTinker &Constructor()
    {
        lua_tinker::class_con<T>(_L, lua_tinker::constructor<T, U...>);
        return *this;
    }

    template <typename T, typename F>
    inline LuaTinker &Function(const char *name, F f)
    {
        lua_tinker::class_def<T>(_L, name, f);
        return *this;
    }

    template <typename T, typename B, typename V>
    inline LuaTinker &Member(const char *name, V B::*v)
    {
        lua_tinker::class_mem<T>(_L, name, v);
        return *this;
    }

    inline LuaState *GetState() { return _L; }

protected:
    LuaState *_L = NULL;
    std::string _name;
};

#define CLASS_CLASS(clazz) class_add<clazz>(#clazz)
#define CLASS_DEF(clazz, member) class_def<clazz>(#member, &clazz::member)
#define CLASS_VAR(clazz, var) class_mem<clazz>(#var, &clazz::member)
#define CLASS_KMEM(clazz, member) class_def<clazz>(#clazz "_" #member, &clazz::member)

} // namespace XSpace

#endif // _LUATINKER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
