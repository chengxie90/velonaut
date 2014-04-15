#include "physics.h"
#include "common.h"
#include "luamanager.h"
#include "app.h"

using namespace std;

Physics::Physics() {}

void Physics::init()
{
    broadphase_ = new btDbvtBroadphase();
    assert(broadphase_);
    collisionConfiguration_ = new btDefaultCollisionConfiguration();
    assert(collisionConfiguration_);
    solver_ = new btSequentialImpulseConstraintSolver;
    assert(solver_);
    dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
    assert(dispatcher_);
    world_ = new btDiscreteDynamicsWorld(Physics::GetInstance()->dispatcher_,
                                                         Physics::GetInstance()->broadphase_,
                                                         Physics::GetInstance()->solver_,
                                                         Physics::GetInstance()->collisionConfiguration_);
    assert(world_);
}

void Physics::initLua()
{
    luaL_Reg reg[] = {   
        {"setWorldGravity", Physics::LWorldSetGravity},
        {"stepWorld", Physics::LWorldStep},
/*
        {"createPhysics", Physics::LPhysicsCreate},
        {"getPhysicsTransform", Physics::LPhysicsGetTransform},
        {"getPhysicsPosition", Physics::LPhysicsGetPosition},
        {"getPhysicsOrientation", Physics::LPhysicsGetOrientation},
        {"setPhysicsTransform", Physics::LPhysicsSetTransform},
        {"setPhysicsPosition", Physics::LPhysicsSetPosition},
        {"setPhysicsOrientation", Physics::LPhysicsSetOrientation},
        {"deletePhysics", Physics::LPhysicsDelete},
            */
        {NULL, NULL}
    };

    LuaManager::GetInstance()->newlib("Physics", reg);
}

void Physics::shutdown()
{
    delete broadphase_;
    delete collisionConfiguration_;
    delete dispatcher_;
    delete solver_;
    delete world_;
}

Physics* Physics::GetInstance()
{
    return App::GetApp()->GetPhysics();
}

int Physics::LWorldSetGravity(lua_State* state)
{
    Physics::GetInstance()->world_->setGravity(btVector3(0,-10,0));
    return 0;
}

int Physics::LWorldStep(lua_State* state)
{
    luaL_checktype(state, -1, LUA_TNUMBER);
    double step = lua_tonumber(state, -1);
    Physics::GetInstance()->world_->stepSimulation(step,1);
    return 0;
}

int Physics::LPhysicsCreate(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsGetTransform(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsGetPosition(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsGetOrientation(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsSetTransform(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsSetPosition(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsSetOrientation(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LPhysicsDelete(lua_State* state)
{
    return 0; // TODO: implement
}
