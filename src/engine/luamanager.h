#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>
#include <lua/lua.hpp>

typedef int (*lua_CFunction) (lua_State *L);

class LuaManager
{
public:
    LuaManager();
    ~LuaManager();

public:
    void LoadScript(std::string file) const;    
    void Call(std::string func, std::string sig, ...) const;
    void Call(std::string func) const;
    void GetParams(std::string params, ...) const;
    void RegisterFunction(const char* name, lua_CFunction func) const;

private:
    void GetMatrixReturn(LUA_NUMBER* result ) const;
    void GetMatrixParam(LUA_NUMBER* result ) const;
    void PushMatrix(LUA_NUMBER*, int numElements)  const;
    void PushVector(LUA_NUMBER*, int numElements)  const;
    void SetReturnValues(const va_list& vl, const std::string params)  const;
    void SetLuaToCParams(const va_list& vl, const std::string params)  const;
    void StackDump()  const;

private:
    lua_State* L;

};

#endif // LUAMANAGER_H
