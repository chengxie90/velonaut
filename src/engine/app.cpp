#include "app.h"
#include <lua/lua.hpp>
#include "graphics.h"
#include "input.h"
#include "luamanager.h"
#include "physics.h"
#include "SDL2/SDL.h"

using namespace std;

extern App g_app;

bool App::init(int argc, char *argv[])
{
    graphics_ = new Graphics;
    input_ = new Input;
    luaManager_ = new LuaManager;
    physics_ = new Physics;
    
    graphics_->init();
    input_->init();
    physics_->init();
    luaManager_->init();

    return true;
}

void App::run()
{
    Uint32 oldTime = SDL_GetTicks();
    
    luaManager_->start();
    
    while (!terminated_) {
        Uint32 curTime = SDL_GetTicks();
        float dt = (curTime - oldTime) / 1000.f;
        oldTime = curTime;
        
        physics_->update(dt);
        input_->update();
        luaManager_->update(dt);
        graphics_->render();
    }
}

void App::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();
    luaManager_->shutdown();
    physics_->shutdown();
    
    delete physics_;
    delete luaManager_;
    delete input_;
    delete graphics_;
}

void App::terminate()
{
    terminated_ = true;
}

App *App::GetApp()
{
    return &g_app;
}

Graphics *App::GetGraphics()
{
    return g_app.graphics_;
}

Input *App::GetInput()
{
    return g_app.input_;
}

LuaManager *App::GetLuaManager()
{
    return g_app.luaManager_;
}

Physics *App::GetPhysics()
{
    return g_app.physics_;
}


