#include "app.h"
#include <lua/lua.hpp>
#include "graphics.h"
#include "input.h"
#include "luamanager.h"
#include "network.h"
#include "ui.h"

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
    ui_ = new Ui();
    network_ = new Network();
    luaManager_ = new LuaManager();
    
    graphics_->init();
    graphics_->scene_->addRenderQueueListener(ui_);

    input_->init();
    ui_->init();
    network_->init();

    luaManager_->init();

    network_->startServer();
    network_->startClient();

    input_->addListener( ui_);

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
    ui_->shutdown();
    
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


