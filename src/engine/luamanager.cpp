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
#include "ui.h"
#include "lua/luaconf.h"

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
    *value = luaL_checknumber(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(double value) const {
    lua_pushnumber(state_, value);
}

void LuaManager::extractParam(double *value)const {
    *value = luaL_checknumber(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(string str) const {
    lua_pushstring(state_, str.c_str());
}

void LuaManager::extractParam(string *str) const {
    *str = luaL_checkstring(state_, 1);
    lua_remove(state_, 1);
}

void LuaManager::addParam(Ogre::Vector3 v) const {

    pushLuaMatrix();

    lua_Number p[3] = {v.x, v.y, v.z};
    pushVector(p, 3);

    pCall(2,1);
}

void LuaManager::extractParam(Ogre::Vector3 *v) const {

    lua_Number result[3];
    removeVector(result, 1);

    v->x = result[0];
    v->y = result[1];
    v->z = result[2];
}

void LuaManager::addParam(Ogre::Matrix3 m) const {

    pushLuaMatrix();

    lua_Number p[9] = {m.GetColumn(0).x, m.GetColumn(1).x, m.GetColumn(2).x,
                       m.GetColumn(0).y, m.GetColumn(1).y, m.GetColumn(2).y,
                       m.GetColumn(0).z, m.GetColumn(1).z, m.GetColumn(2).z,};
    pushMatrix(p, 9);

    pCall(2, 1);
    
}

void LuaManager::addParam(void *p) const
{
    lua_pushlightuserdata(state_, p);
}

void LuaManager::extractParam(Ogre::Matrix3 *m) const {

    lua_Number result[9];
    removeMatrix(result, 1);

    m->SetColumn(0, Ogre::Vector3(result[0],result[3], result[6]));
    m->SetColumn(1, Ogre::Vector3(result[1],result[4], result[7]));
    m->SetColumn(2, Ogre::Vector3(result[2],result[5], result[8]));
}

void LuaManager::extractParam(Ogre::ColourValue *c) const
{
    lua_Number result[4] = {0, 0, 0, 1};
    removeVector(result, 1);
    c->r = result[0];
    c->g = result[1];
    c->b = result[2];
    c->a = result[3];
}

void LuaManager::pushVector(lua_Number* matrix, int num_elements) const {

    lua_newtable(state_);

    for(int i = 0; i < num_elements; i++) {
        lua_pushnumber(state_, matrix[i]);
        lua_rawseti(state_, -2, i+1);
    }
}

void LuaManager::removeVector(lua_Number* result, int index) const {

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

void LuaManager::pushMatrix(lua_Number* matrix, int num_elements) const {

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

void LuaManager::removeMatrix(lua_Number* result, int index) const {
    luaL_checktype(state_, index, LUA_TTABLE);
    int len = 0; // TODO (state_, index);

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

    Ui::GetInstance()->initLua();
}

void LuaManager::start()
{
    doFile("./data/scripts/engine/app.lua");
}

void LuaManager::update(float dt)
{
    addFunction("App.update");
    addParam((double)dt);
    pCall(1);
}

void LuaManager::shutdown()
{
    lua_close(state_);
}

void LuaManager::addlib(luaL_Reg* reg)
{
    luaL_newlib(state_, reg);
}

void LuaManager::requiref(string name, lua_CFunction func)
{
    luaL_requiref(state_, name.c_str(), func, false);
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

void LuaManager::extractParam(void **p) const
{
    luaL_checktype(state_, 1, LUA_TLIGHTUSERDATA);
    *p = lua_touserdata(state_, 1); 
    lua_remove(state_, 1);
}
