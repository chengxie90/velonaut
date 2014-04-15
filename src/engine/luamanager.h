#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <lua/lua.hpp>
#include "common.h"

class LuaManager
{
public:
    void init();
    void update();
    void shutdown();
    
    void newlib(string libname, luaL_Reg reg[]);
    
    void dofile(string file) const;
    void SetMatrixParam(LUA_NUMBER*, int numElements)  const;
    void pcall(string func, string sig, ...) const;
    void pcall(string func) const;
    void GetMatrixParam(LUA_NUMBER* result ) const;
    void RegisterFunction(const char* name, lua_CFunction func) const;

    lua_State *state() const;
    
    static LuaManager* GetInstance();
    
private:
    friend class App;
    SINGLETON(LuaManager)
    
    lua_State* state_;
};

#endif // LUAMANAGER_H
