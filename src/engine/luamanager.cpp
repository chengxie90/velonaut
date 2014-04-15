#include <string>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <cmath>
#include "luamanager.h"
#include "app.h"
#include "graphics.h"

using namespace std;

LuaManager::LuaManager() {}

void LuaManager::SetMatrixParam(LUA_NUMBER* matrix, int num_elements) const {
    lua_newtable(state_);
    int len = sqrt(num_elements);

    for(int row = 0; row < len; row++) {

        for(int col = 0; col < len; col++) {

            lua_pushnumber(state_, matrix[row*len+col]);
            lua_rawseti(state_, -2, row*len+col+1);

        }
    }
}

void LuaManager::GetMatrixParam(LUA_NUMBER* result) const
{
    luaL_checktype(state_, -1, LUA_TTABLE);
    int len = 0; //lua_objlen(state_, -1);

    for(int row = 0; row < len; row++) {
        lua_pushinteger(state_, row+1);
        lua_gettable(state_, -2);

        for(int col = 0; col < len; col++) {
            lua_pushinteger(state_, col+1);
            lua_gettable(state_, -2);

            if(lua_isnumber(state_, -1)) {

                result[row*len+col] = lua_tonumber(state_, -1);

            } else {
                std::cout << "not a number!" << std::endl;
            }
            lua_pop(state_, 1);
        }
        lua_pop(state_, 1);
    }
}

void LuaManager::pcall(std::string func) const {
    pcall(func, "");
}

void LuaManager::pcall(std::string func, std::string sig, ...) const {

    va_list vl;
    int narg, nres;

    // determine number of arguments and return values
    narg = sig.substr(0, sig.find(">")).size();
    nres = (narg < sig.size() ? sig.size()-narg-1 : 0);

    va_start(vl, sig.c_str() );

    // allow direct object access, if requested. e.g. 'App.init'
    size_t pos = func.find(".");
    if (pos != std::string::npos) {
        std::string table = func.substr (0, pos);
        std::string fname = func.substr (pos+1);

        lua_getglobal(state_, table.c_str());
        lua_getfield(state_, -1, fname.c_str());

    } else {
        lua_getglobal(state_, func.c_str() );
    }

    // push function arguments on stack
    int i = 0;
    for (; i < sig.size(); ++i) {
        luaL_checkstack(state_, 1, "too many arguments");

        switch(sig.at(i)) {
        case 'd':
            lua_pushnumber(state_, va_arg(vl, double));
            break;
        case 'i':
            lua_pushnumber(state_, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(state_, va_arg(vl, char *));
            break;
        case 'm':
            SetMatrixParam(va_arg(vl, LUA_NUMBER*), 9);
            break;
        case '>':
            goto endargs;
            break;

        default:
            std::cout << "Error: Unhandled argument type " << sig.at(i) << std::endl;
        }
    }

    endargs:

    // call Lua function
    if (lua_pcall(state_, narg, nres, 0) != 0) {
        std::string str = lua_tostring(state_, lua_gettop(state_));
        lua_pop(state_, 1);
        std::cout << str << std::endl;
        assert(false);
    }

    // retrieve return values
    i++; // jump over '>'
    nres = -nres;
    for (; i < sig.size(); ++i) {
        luaL_checkstack(state_, 1, "too many arguments");

        switch(sig.at(i)) {
        case 'd':
        {
            int isnum;
            double n = lua_tonumberx(state_, nres, &isnum);
            *va_arg(vl, double *) = n;
            break;
        }
        case 'i':
        {
            int isnum;
            double n = lua_tointegerx(state_, nres, &isnum);
            *va_arg(vl, int *) = n;
            break;
        }
        case 's':
        {
            const char *s = lua_tostring(state_, nres);
            *va_arg(vl, const char **) = s;
            break;
        }
        case 'm':
        {
            GetMatrixParam(va_arg(vl, LUA_NUMBER*));
            break;
        }
        default:
            std::cout << "Error: Unhandled return type " << sig.at(i) << std::endl;
            break;
        }

        ++nres;
    }

    va_end(vl);
}

void LuaManager::RegisterFunction(const char *name, lua_CFunction func) const {
    lua_register(state_, name, func);
}

lua_State *LuaManager::state() const
{
    return state_;
}

LuaManager *LuaManager::GetInstance()
{
    return App::GetApp()->GetLuaManager();
}

void LuaManager::init()
{
    state_ = luaL_newstate();
    assert(state_);
    luaL_openlibs(state_);

    Graphics::GetInstance()->initLua();
    
    dofile("./data/scripts/app.lua");
        
    pcall("App.init");    
}

void LuaManager::update()
{
    
}

void LuaManager::shutdown()
{
    lua_close(state_);
}

void LuaManager::newlib(string libname, luaL_Reg reg[])
{
    luaL_newlib(state_, reg);
    lua_setglobal(state_, libname.c_str());
}

void LuaManager::dofile(string file) const {
    int error = luaL_dofile(state_, file.c_str());
    if (error)
    {
        std::string str = lua_tostring(state_, lua_gettop(state_));
        lua_pop(state_, 1);
        std::cout << str << std::endl;
        assert(false);
    }
}
