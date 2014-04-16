#include "physics.h"
#include "common.h"
#include "luamanager.h"
#include "app.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
//
// COMPOUND COLLIDER COMPONENT STUFF

void Physics::InitLuaSimpleCollider()
{
    luaL_Reg reg[] = {
        {"createCompoundCollider", LCompoundColliderCreate},
        {"addChildCollider", LCompoundColliderAddChild},
        {"getChildColliderByIndex", LCompoundColliderGetChildByIndex},
        {"getNumChildColliders", LCompoundColliderGetNumChildren},
        {"removeChildColliderByIndex", LCompoundColliderRemoveChildByIndex},
        {"setScaling", LCompoundColliderSetLocalScaling},
        {"getScaling", LCompoundColliderGetLocalScaling},
        {"deleteCollider", LCompoundColliderDelete},
        {NULL, NULL}
    };

    LuaManager::GetInstance()->newlib("CompoundCollider", reg);
}

int Physics::LCompoundColliderCreate(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderAddChild(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderGetChildByIndex(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderGetNumChildren(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderRemoveChildByIndex(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderGetLocalScaling(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderSetLocalScaling(lua_State* state)
{
    return 0; // TODO: implement
}

int Physics::LCompoundColliderDelete(lua_State* state)
{
    return 0; // TODO: implement
}
