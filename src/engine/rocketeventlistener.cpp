#include <iostream>
#include "rocketeventlistener.h"
#include "luamanager.h"

RocketEventListener::RocketEventListener(int r):r(r)
{
}

void RocketEventListener::ProcessEvent(Rocket::Core::Event &event) {
    lua_rawgeti(LuaManager::GetInstance()->state(), LUA_REGISTRYINDEX, r);
    LuaManager::GetInstance()->pCall();
}

void RocketEventListener::OnAttach(Rocket::Core::Element* ROCKET_UNUSED(element)) {
    std::cout << "onAttach" << std::endl;
}

void RocketEventListener::OnDetach(Rocket::Core::Element* ROCKET_UNUSED(element)) {
    std::cout << "onDetach" << std::endl;
}

