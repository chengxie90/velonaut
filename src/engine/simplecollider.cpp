#include "physics.h"
#include "common.h"
#include "luamanager.h"
#include "app.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
//
// SIMPLE COLLIDER COMPONENT STUFF

/*
void Physics::InitLuaSimpleCollider()
{
    luaL_Reg reg[] = {
        {"createSphereCollider", Physics::LSimpleColliderCreateSphere},
        {"createBoxCollider", Physics::LSimpleColliderCreateBox},
        {"createCylinderCollider", Physics::LSimpleColliderCreateCylinder},
        {"createCapsuleCollider", Physics::LSimpleColliderCreateCapsule},
        {"getScaling", Physics::LSimpleColliderGetLocalScaling},
        {"setScaling", Physics::LSimpleColliderSetLocalScaling},
        {"deleteCollider", Physics::LSimpleColliderDelete},
        {NULL, NULL}
    };

    LuaManager::GetInstance()->newlib("SimpleCollider", reg);
}
*/

int Physics::LSimpleColliderCreateSphere(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderCreateBox(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderCreateCylinder(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderCreateCapsule(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderGetLocalScaling(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderSetLocalScaling(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LSimpleColliderDelete(lua_State* state)
{
    return 0; // TODO: implement
}
