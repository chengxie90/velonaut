#include "app.h"
#include <lua/lua.hpp>
#include "graphics.h"
#include "input.h"
#include "luamanager.h"
#include "network.h"

using namespace std;

extern App g_app;


static int LScript_Bla(lua_State* state) {

    double d;
    string s;

    App::GetLuaManager()->extractParam(&d);
    App::GetLuaManager()->extractParam(&s);
    std::cout << "got from lua: " << d << std::endl;
    std::cout << "got from lua: " << s << std::endl;

    App::GetLuaManager()->addParam(45);
    App::GetLuaManager()->addParam("hello from c");
    return 2;
}

bool App::init(int argc, char *argv[])
{
    graphics_ = new Graphics();
    input_ = new Input();
    luaManager_ = new LuaManager();
    network_ = new Network();
    
    graphics_->init();
    input_->init();
    luaManager_->init();

    network_->startServer();
    sleep(0.5);
    network_->startClient();

    return true;
}

void App::run()
{
    while (!terminated_) {
        input_->update();
        luaManager_->update();
        network_->poll();
        graphics_->render();
    }
}

void App::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();
    luaManager_->shutdown();
    
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


