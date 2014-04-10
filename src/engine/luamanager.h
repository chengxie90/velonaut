#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>
#include <lua/lua.hpp>

typedef int (*lua_CFunction) (lua_State *L);

class LuaManager
{
public:
    LuaManager();
    lua_State* L;

private:
    void load(std::string file) const;
};

#endif // LUAMANAGER_H
