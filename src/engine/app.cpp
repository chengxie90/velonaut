#include "app.h"
#include <lua/lua.hpp>
#include "graphics.h"
#include "input.h"
#include "luamanager.h"
#include "network.h"
#include "ui.h"
#include "physics.h"
#include "SDL2/SDL.h"


using namespace std;

extern App g_app;

bool App::init(int argc, char *argv[])
{

    input_ = new Input;
    graphics_ = new Graphics;
    luaManager_ = new LuaManager;
    physics_ = new Physics;
    ui_ = new Ui;
    network_ = new Network;
    luaManager_ = new LuaManager;
    
    graphics_->init();
    input_->init();
    physics_->init();
    ui_->init();
    network_->init();
    luaManager_->init();

    input_->addListener( ui_);

    return true;
}

void App::run()
{
    Uint32 oldTime = SDL_GetTicks();
    
    luaManager_->start();
    graphics_->scene_->addRenderQueueListener(ui_);
    
    while (!terminated_) {
        Uint32 curTime = SDL_GetTicks();
        float dt = (curTime - oldTime) / 1000.f;
        oldTime = curTime;
        
        physics_->update(dt);
        input_->update();
        luaManager_->update(dt);
        network_->poll();
        graphics_->render();
    }
}

void App::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();
    luaManager_->shutdown();
    ui_->shutdown();
    physics_->shutdown();

    
    delete physics_;
    delete luaManager_;
    delete input_;
    delete graphics_;
    delete ui_;
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

Ui *App::GetUi()
{
    return g_app.ui_;
}

Network *App::GetNetwork()
{
    return g_app.network_;
}

Physics *App::GetPhysics()
{
    return g_app.physics_;
}


