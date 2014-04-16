#include "gametest.h"
#include <lua/lua.hpp>
#include "graphics.h"
#include "input.h"
#include "luamanager.h"

using namespace std;

extern GameTest g_app;

bool GameTest::init(int argc, char *argv[])
{
    graphics_ = new Graphics();
    input_ = new Input();
    luaManager_ = new LuaManager();

    graphics_->init();
    input_->init();
    luaManager_->init();

    luaManager_->Call("App.update", "s", "chello");

    return true;
}

void GameTest::run()
{
    while (!terminated_) {
        input_->update();
        luaManager_->update();
        graphics_->render();
    }
}

void GameTest::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();
    luaManager_->shutdown();

    delete luaManager_;
    delete input_;
    delete graphics_;
}

void GameTest::terminate()
{
    terminated_ = true;
}

GameTest *GameTest::GetApp()
{
    return &g_app;
}

Graphics *GameTest::GetGraphics()
{
    return g_app.graphics_;
}

Input *GameTest::GetInput()
{
    return g_app.input_;
}

LuaManager *GameTest::GetLuaManager()
{
    return g_app.luaManager_;
}

Physics *GameTest::GetPhysics()
{
    return g_app.physics_;
}

