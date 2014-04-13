#include "app.h"
#include "ogretest.h"
#include "luamanager.h"

bool App::init()
{
    test_ = new OgreTest();
    test_->init();

    LuaManager luaManager;
    luaManager.LoadScript("data/scripts/app.lua");
    luaManager.Call("App.init");

    LUA_NUMBER m[9];
    luaManager.Call("hello", "ddm>m", 1.0, 2.0, m, &m);

    for (int i = 0; i < 9;++i) {
        std::cout << "m: " << m[i] << std::endl;
    }
}

void App::run()
{
#ifndef __APPLE__
    test_->run();
#endif
}

#ifdef __APPLE__
void App::render()
{
    test_->render();
}
#endif

#include "app.h"
