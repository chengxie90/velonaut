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
    //void render();
    void shutdown();

    static Physics* GetInstance();
private:
    SINGLETON(Physics)
    friend class App;

    // tmp
    void createWorld();

    btBroadphaseInterface* broadphase_ = NULL;
    btDefaultCollisionConfiguration* collisionConfiguration_ = NULL;
    btCollisionDispatcher* dispatcher_ = NULL;
    btSequentialImpulseConstraintSolver* solver_ = NULL;
    btDynamicsWorld* world_ = NULL;

private:
    // Lua setup
    void InitLuaPhysics();
    void InitLuaSimpleCollider();
    void InitLuaCompoundCollider();

    // World stuff
    static int LWorldSetGravity(lua_State* state);
    static int LWorldStep(lua_State* state);

    // Physics stuff
    static int LPhysicsCreate(lua_State* state);
    static int LPhysicsGetTransform(lua_State* state);
    static int LPhysicsGetPosition(lua_State* state);
    static int LPhysicsGetOrientation(lua_State* state);
    static int LPhysicsSetTransform(lua_State* state);
    static int LPhysicsSetPosition(lua_State* state);
    static int LPhysicsSetOrientation(lua_State* state);
    static int LPhysicsDelete(lua_State* state);

    // Simple collider stuff
    static int LSimpleColliderCreateSphere(lua_State* state);
    static int LSimpleColliderCreateBox(lua_State* state);
    static int LSimpleColliderCreateCylinder(lua_State* state);
    static int LSimpleColliderCreateCapsule(lua_State* state);
    static int LSimpleColliderGetLocalScaling(lua_State* state);
    static int LSimpleColliderSetLocalScaling(lua_State* state);
    static int LSimpleColliderDelete(lua_State* state);

    // Compound collider stuff
    static int LCompoundColliderCreate(lua_State* state);
    static int LCompoundColliderAddChild(lua_State* state);
    static int LCompoundColliderGetChildByIndex(lua_State* state);
    static int LCompoundColliderGetNumChildren(lua_State* state);
    static int LCompoundColliderRemoveChildByIndex(lua_State* state);
    static int LCompoundColliderGetLocalScaling(lua_State* state);
    static int LCompoundColliderSetLocalScaling(lua_State* state);
    static int LCompoundColliderDelete(lua_State* state);
};

#endif // PHYSICS_H
