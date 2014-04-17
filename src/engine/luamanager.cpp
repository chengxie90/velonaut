#include <string>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <cmath>
#include <exception>
#include <assert.h>
#include "luamanager.h"
#include "app.h"
#include "graphics.h"

using namespace std;

LuaManager::LuaManager() {}

void LuaManager::addFunction(string func) const {

    size_t pos = func.find(".");
    if (pos != std::string::npos) {
        std::string table = func.substr (0, pos);
        std::string fname = func.substr (pos+1);

        lua_getglobal(state_, table.c_str());
        lua_getfield(state_, -1, fname.c_str());
        lua_remove(state_, -2);

    } else {
        lua_getglobal(state_, func.c_str() );
    }
}

void LuaManager::pCall(int nargs, int nresults) const {

    if (lua_pcall(state_, nargs, nresults, 0) != 0) {
        std::string str = lua_tostring(state_, lua_gettop(state_));
        lua_pop(state_, 1);
        std::cout << str << std::endl;
        assert(false);
    }
}

void LuaManager::addParam(int value) const {
    lua_pushnumber(state_, value);
}

void LuaManager::extractParam(int *value) const {
    *value = lua_tonumber(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(double value) const {
    lua_pushnumber(state_, value);
}

void LuaManager::extractParam(double *value)const {
    *value = lua_tonumber(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(string str) const {
    lua_pushlstring(state_, str.c_str(), str.size());
}

void LuaManager::extractParam(string *str) const {
    *str = lua_tostring(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(Ogre::Vector3 v) const {

    pushLuaMatrix();

    LUA_NUMBER p[3] = {v.x, v.y, v.z};
    pushVector(p, 3);

    pCall(2,1);
}

void LuaManager::extractParam(Ogre::Vector3 *v) const {

    LUA_NUMBER result[3];
    removeVector(result, 1);

    v->x = result[0];
    v->y = result[1];
    v->z = result[2];
}

void LuaManager::addParam(Ogre::Matrix3 m) const {

    pushLuaMatrix();

    LUA_NUMBER p[9] = {m.GetColumn(0).x, m.GetColumn(1).x, m.GetColumn(2).x,
                       m.GetColumn(0).y, m.GetColumn(1).y, m.GetColumn(2).y,
                       m.GetColumn(0).z, m.GetColumn(1).z, m.GetColumn(2).z,};
    pushMatrix(p, 9);

    pCall(2, 1);

}

void LuaManager::extractParam(Ogre::Matrix3 *m) const {

    LUA_NUMBER result[9];
    removeMatrix(result, 1);

    m->SetColumn(0, Ogre::Vector3(result[0],result[3], result[6]));
    m->SetColumn(1, Ogre::Vector3(result[1],result[4], result[7]));
    m->SetColumn(2, Ogre::Vector3(result[2],result[5], result[8]));
}

void LuaManager::pushVector(LUA_NUMBER* matrix, int num_elements) const {

    lua_newtable(state_);

    for(int i = 0; i < num_elements; i++) {

        lua_pushnumber(state_, matrix[i]);
        lua_rawseti(state_, -2, i+1);

    }
}

void LuaManager::removeVector(LUA_NUMBER* result, int index) const {

    luaL_checktype(state_, index, LUA_TTABLE);
    int len = lua_rawlen(state_, index);

    for(int row = 0; row < len; row++) {
        lua_pushinteger(state_, row+1);
        lua_gettable(state_, -2);

        lua_pushinteger(state_, 1);
        lua_gettable(state_, -2);

        if(lua_isnumber(state_, -1)) {

            result[row] = lua_tonumber(state_, -1);

        } else {
            std::cout << "not a number!" << std::endl;
            assert(false);
        }
        lua_pop(state_, 1);
        lua_pop(state_, 1);
    }

    lua_remove(state_, index);
}

void LuaManager::pushMatrix(LUA_NUMBER* matrix, int num_elements) const {

    lua_newtable(state_);
    int len = sqrt(num_elements);

    for(int row = 0; row < len; row++) {

        lua_newtable(state_);

        for(int col = 0; col < len; col++) {

            lua_pushnumber(state_, matrix[row*len+col]);
            lua_rawseti(state_, -2, col+1);

        }

        lua_rawseti(state_, -2, row+1);
    }
}

void LuaManager::removeMatrix(LUA_NUMBER* result, int index) const {
    luaL_checktype(state_, index, LUA_TTABLE);
    int len = lua_objlen(state_, index);

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

    lua_remove(state_, index);
}

void LuaManager::pushLuaMatrix() const {
    lua_getglobal(state_, "Matrix");
    lua_getfield(state_, -1, "new");
    lua_remove(state_,-2);
    lua_getglobal(state_, "Matrix");
}

void LuaManager::dumpStack() const {

    int i;
    int top = lua_gettop(state_);
    std::cout << "==== STACKDUMP len:" << top << " ===" << std::endl;
    for (i = top; i >= 1; --i) {
        int t = lua_type(state_,i);
        switch(t) {
        case LUA_TSTRING:
        {
            std::cout << lua_tostring(state_, i) << std::endl;
            break;
        }
        case LUA_TBOOLEAN:
        {
            std::cout << (lua_toboolean(state_,i) ? "true" : "false") << std::endl;
            break;
        }
        case LUA_TNUMBER:
        {
            std::cout << lua_tonumber(state_, i) << std::endl;
            break;
        }
        default:
        {
            std::cout << lua_typename(state_,t) << std::endl;
        }
        }
    }
    std::cout << "========================" << std::endl;
}


void LuaManager::registerFunction(const char *name, lua_CFunction func) const {
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

    doFile("./data/scripts/app.lua");
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

void LuaManager::doFile(string file) const {
    int error = luaL_dofile(state_, file.c_str());
    if (error)
    {
        std::string str = lua_tostring(state_, lua_gettop(state_));
        lua_pop(state_, 1);
        std::cout << str << std::endl;
        assert(false);
    }
}

