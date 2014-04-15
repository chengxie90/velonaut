#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <lua/lua.hpp>
#include "common.h"

class LuaManager
{
public:
    void LoadScript(std::string file) const;    
    void Call(std::string func, std::string sig, ...) const;
    void Call(std::string func) const;
    void GetParams(std::string params, ...) const;

    void init();
    void update();
    void shutdown();
    
    void newlib(string libname, luaL_Reg reg[]);
    
    void dofile(string file) const;
    void SetMatrixParam(LUA_NUMBER*, int numElements)  const;
    void GetMatrixParam(LUA_NUMBER* result ) const;

    void RegisterFunction(const char* name, lua_CFunction func) const;

    lua_State *state() const;
    
    static LuaManager* GetInstance();
    
private:

    void GetMatrixReturn(LUA_NUMBER* result ) const;
    void PushMatrix(LUA_NUMBER*, int numElements)  const;
    void PushVector(LUA_NUMBER*, int numElements)  const;
    void SetReturnValues(const va_list& vl, const std::string params)  const;
    void SetLuaToCParams(const va_list& vl, const std::string params)  const;
    void StackDump()  const;

    lua_State* L;

    friend class App;
    SINGLETON(LuaManager)
    
    //lua_State* state_;
};

#endif // LUAMANAGER_H
