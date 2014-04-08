#include <lua/lua.hpp>
#include <string>
#include <iostream>
#include "luamanager.h"

typedef int (*lua_CFunction) (lua_State *L);

static int lscript_print(lua_State *L)
{

    std::string param = lua_tostring(L, 1);
    std::cout << param << std::endl;
    return 0;
}

LuaManager::LuaManager()
{

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "lscript_print", lscript_print);
    int error = luaL_dofile(L, "data/scripts/luatest.lua");

    if(error) // if non-0, then an error
    {
        // get the top of the stack as the error and pop it off
        std::string str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);

        std::cout << str << std::endl;
    }

    lua_close(L);
}
