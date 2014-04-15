#include <string>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <cmath>
#include <exception>
#include <assert.h>
#include "luamanager.h"

LuaManager::LuaManager(){
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaManager::~LuaManager(){
    lua_close(L);
}

void LuaManager::GetParams(std::string params, ...) const {
    int num_args = lua_gettop(L);

    if (num_args != params.size()) {
        std::cout << "number of parameters much match the number of actual return values" << std::endl;
        assert(false);
        return;
    }

    va_list vl;
    va_start(vl, params.c_str() );
    SetLuaToCParams(vl, params);
    va_end(vl);
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

            // create new matrix object and store it on stack as argument

            lua_getglobal(L, "Matrix");
            lua_getfield(L, -1, "new");
            lua_remove(L,-2);

            lua_getglobal(L, "Matrix");
            PushMatrix(va_arg(vl, LUA_NUMBER*), 9);

            if (lua_pcall(L, 2, 1, 0) != 0) {
                std::cout << "error" << std::endl;
                lua_error(L);
            }

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

    // Set return values passed
    if (nres > 0)
        SetReturnValues(vl, sig.substr(++i));

    va_end(vl);
}

void LuaManager::PushVector(LUA_NUMBER* matrix, int num_elements) const {

    lua_newtable(L);
    int len = sqrt(num_elements);

    for(int row = 0; row < len; row++) {

        lua_newtable(L);

        for(int col = 0; col < len; col++) {

            lua_pushnumber(L, matrix[row*len+col]);
            lua_rawseti(L, -2, col+1);

        }

        lua_rawseti(L, -2, row+1);
    }
}

void LuaManager::PushMatrix(LUA_NUMBER* matrix, int num_elements) const {

    lua_newtable(L);
    int len = sqrt(num_elements);

    for(int row = 0; row < len; row++) {

        lua_newtable(L);

        for(int col = 0; col < len; col++) {

            lua_pushnumber(L, matrix[row*len+col]);
            lua_rawseti(L, -2, col+1);

        }

        lua_rawseti(L, -2, row+1);
    }
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

void LuaManager::SetLuaToCParams(const va_list& vl, const std::string params) const {

    int na = 1;

    for( int i = 0; i < params.size(); ++i) {

        char c = params.at(i);
        switch (c) {
        case 'd':
        {
            double d = lua_tonumber(L, na);
            *va_arg(vl, double *) = d;
            break;
        }
        case 'i':
        {
            double n = lua_tonumber(L, na);
            *va_arg(vl, int *) = n;
            break;
        }
        case 's':
        {
            const char *s = lua_tostring(L, na);
            *va_arg(vl, const char **) = s;
            break;
        }
        case 'm':
        {
            GetMatrixParam(va_arg(vl, LUA_NUMBER*));
            break;
        }
        }
        ++na;
    }
}

void LuaManager::SetReturnValues(const va_list& vl, const std::string params) const {

    int na = -params.size();

    for( int i = 0; i < params.size(); ++i) {

        StackDump();
        char c = params.at(i);
        switch (c) {
        case 'd':
        {
            double d = lua_tonumber(L, na);
            *va_arg(vl, double *) = d;
            break;
        }
        case 'i':
        {
            double n = lua_tonumber(L, na);
            *va_arg(vl, int *) = n;
            break;
        }
        case 's':{
            const char *s = lua_tostring(L, na);
            *va_arg(vl, const char **) = s;
            break;
        }
        case 'm':
        {
            GetMatrixReturn(va_arg(vl, LUA_NUMBER*));
            break;
        }
        }
        lua_remove(L, na);
        ++na;
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

void LuaManager::GetMatrixReturn(LUA_NUMBER* result) const
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int len = lua_objlen(L, 1);

    for(int row = 0; row < len; row++) {
        lua_pushinteger(L, row+1);
        lua_gettable(L, 1);

        for(int col = 0; col < len; col++) {
            lua_pushinteger(L, col+1);
            lua_gettable(L, -2);

            if(lua_isnumber(L, -1)) {
                int isNum;
                result[row*len+col] = lua_tonumberx(L, -1, &isNum);

            } else {
                std::cout << "not a number!" << std::endl;
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
}

void LuaManager::StackDump() const {

    int i;
    int top = lua_gettop(L);
    std::cout << "==== STACKDUMP len:" << top << " ===" << std::endl;
    for (i = top; i >= 1; --i) {
        int t = lua_type(L,i);
        switch(t) {
        case LUA_TSTRING:
        {
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN:
        {
            printf(lua_toboolean(L,i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER:
        {
            printf("'%g'", lua_tonumber(L, i));
            break;
        }
        default:
        {
            printf("'%s'", lua_typename(L, t));
        }
        }
        printf("\n");
    }
    printf("========================\n");
}
