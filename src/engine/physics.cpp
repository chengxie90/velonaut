#include "physics.h"
#include "common.h"
#include "luamanager.h"
#include "app.h"

using namespace std;

Physics::Physics() {}

void Physics::init()
{

}

void Physics::initLua()
{
    luaL_Reg reg[] = {
        {"createWorld", Graphics::LWorldCreate},
        {NULL, NULL}
    };

    LuaManager::GetInstance()->newlib("Physics", reg);
}
