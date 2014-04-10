#include <string>
#include <iostream>
#include "luamanager.h"

static int lscript_print(lua_State *L)
{
    std::string param = lua_tostring(L, 1);
    std::cout << param << std::endl;
    return 0;
}

LuaManager::LuaManager():L(0)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "lscript_print", lscript_print);
    load("data/scripts/app.lua");

    lua_close(L);
}

void LuaManager::load(std::string file) const {
    int error = luaL_dofile(L, file.c_str());

    if(error)
    {
        std::string str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);
        std::cout << str << std::endl;
    }
}
