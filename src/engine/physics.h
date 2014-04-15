#ifndef PHYSICS_H
#define PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>
#include <lua/lua.hpp>
#include "common.h"

class Physics
{
public:
    void init();
    void initLua();
    void render();
    void shutdown();

    static Physics* GetInstance();
private:
    SINGLETON(Physics)
    friend class App;

    void initWindow();
    void initResources();

    // tmp
    void createWorld();

    btDynamicsWorld* world = NULL;

private:
    // World stuff
    static int LWorldCreate(lua_State* L);
    static int LWorldSetGravity(lua_State* L);
    static int LWorldStep(lua_State* L);

    // Rigid body stuff
    static int LPhysicsCreate(lua_State* L);
    static int LPhysicsGetTransform(lua_State* L);
    static int LPhysicsGetPosition(lua_State* L);
    static int LPhysicsGetOrientation(lua_State* L);
    static int LPhysicsSetTransform(lua_State* L);
    static int LPhysicsSetPosition(lua_State* L);
    static int LPhysicsSetOrientation(lua_State* L);
    static int LPhysicsDelete(lua_State* L);

    // Simple collision shape stuff
    static int LSimpleColliderCreateSphere(lua_State* L);
    static int LSimpleColliderCreateBox(lua_State* L);
    static int LSimpleColliderCreateCylinder(lua_State* L);
    static int LSimpleColliderCreateCapsule(lua_State* L);
    static int LSimpleColliderSetLocalScaling(lua_State* L);
    static int LSimpleColliderDelete(lua_State* L);

    // Compound collision shape stuff
    static int LCompoundColliderCreate(lua_State* L);
    static int LCompoundColliderAddChild(lua_State* L);
    static int LCompoundColliderGetChildByIndex(lua_State* l);
    static int LCompoundColliderRemoveChildByIndex(lua_State* L);
    static int LCompoundColliderGetNumChildren(lua_State* L);
    static int LCompoundColliderSetLocalScaling(lua_State* L);
    static int LCompoundColliderDelete(lua_State* L);
};

#endif // PHYSICS_H
