#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <lua/lua.hpp>
#include <Ogre.h>
#include "input.h"
#include "common.h"
#include "BulletDynamics/btBulletDynamicsCommon.h"

class LuaManager : public InputListener
{
public:
    void init();
    void start();
    void update(float dt);
    void shutdown();

    void doFile(std::string file) const;
    void addlib(luaL_Reg *reg);
    void requiref(std::string name, lua_CFunction func);
    void pCall(int nargs = 0, int nresults = 0) const;
    void call(int nargs = 0, int nres = 0) const;
    void registerFunction(const char* name, lua_CFunction func) const;
    
    void dumpStack() const;

    void addFunction(std::string str) const;

    void addParam(int value) const;
    void addParam(double value) const;
    void addParam(std::string str) const;
    void addParam(const btVector3& v) const;
    void addParam(const btQuaternion& q) const;
    void addParam(const Ogre::Vector3& v) const;
    void addParam(const Ogre::Quaternion& q) const;
    void addParam(void *) const;
    
    void extractParam(int *value) const;
    void extractParam(double *value) const;
    void extractParam(std::string *str) const;
    void extractParam(btVector3 *v) const;
    void extractParam(btQuaternion *q) const;
    void extractParam(Ogre::Quaternion *q) const;
    void extractParam(Ogre::Vector3 *v) const;
    void extractParam(Ogre::ColourValue *c) const;
    void extractParam(void**) const;
    
    void addParamReg(const void *p) const;
    void setReg(const void *p) const;
    
    virtual void onMouseDown( SDL_Event e );
    virtual void onMouseUp( SDL_Event e);
    virtual void onMouseMove( SDL_Event e );
    virtual void onKeyDown( SDL_Event e );
    virtual void onKeyUp( SDL_Event e );

    lua_State *state() const;
    
    static LuaManager* GetInstance();
private:
    lua_State* state_;

    friend class App;
    SINGLETON(LuaManager)
    
    void addParam(lua_Number* array, int len) const;
    void extractParam(lua_Number* array, int len) const;

    void AddDictionary();
};

#endif // LUAMANAGER_H
