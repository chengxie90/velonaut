#include <string>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <cmath>
#include "luamanager.h"

LuaManager::LuaManager(){
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaManager::~LuaManager(){
    lua_close(L);
}

void LuaManager::SetMatrixParam(LUA_NUMBER* matrix, int num_elements) const {
    lua_newtable(L);
    int len = sqrt(num_elements);

    for(int row = 0; row < len; row++) {

        for(int col = 0; col < len; col++) {

            lua_pushnumber(L, matrix[row*len+col]);
            lua_rawseti(L, -2, row*len+col+1);

        }
    }
}

void LuaManager::GetMatrixParam(LUA_NUMBER* result) const
{
    luaL_checktype(L, -1, LUA_TTABLE);
    int len = lua_objlen(L, -1);

    for(int row = 0; row < len; row++) {
        lua_pushinteger(L, row+1);
        lua_gettable(L, -2);

        for(int col = 0; col < len; col++) {
            lua_pushinteger(L, col+1);
            lua_gettable(L, -2);

            if(lua_isnumber(L, -1)) {

                result[row*len+col] = lua_tonumber(L, -1);

            } else {
                std::cout << "not a number!" << std::endl;
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
}

void LuaManager::Call(std::string func) const {
    Call(func, "");
}

void LuaManager::Call(std::string func, std::string sig, ...) const {

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

        lua_getglobal(L, table.c_str());
        lua_getfield(L, -1, fname.c_str());

    } else {
        lua_getglobal(L, func.c_str() );
    }

    // push function arguments on stack
    int i = 0;
    for (; i < sig.size(); ++i) {
        luaL_checkstack(L, 1, "too many arguments");

        switch(sig.at(i)) {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushnumber(L, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl, char *));
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
    if (lua_pcall(L, narg, nres, 0) != 0) {
        std::cout << "error" << std::endl;
        lua_error(L);
    }

    // retrieve return values
    i++; // jump over '>'
    nres = -nres;
    for (; i < sig.size(); ++i) {
        luaL_checkstack(L, 1, "too many arguments");

        switch(sig.at(i)) {
        case 'd':
        {
            int isnum;
            double n = lua_tonumberx(L, nres, &isnum);
            *va_arg(vl, double *) = n;
            break;
        }
        case 'i':
        {
            int isnum;
            double n = lua_tointegerx(L, nres, &isnum);
            *va_arg(vl, int *) = n;
            break;
        }
        case 's':
        {
            const char *s = lua_tostring(L, nres);
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
    lua_register(L, name, func);
}

void LuaManager::LoadScript(std::string file) const {
    int error = luaL_dofile(L, file.c_str());

    if(error)
    {
        std::string str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);
        std::cout << str << std::endl;
    }
}
