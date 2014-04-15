#include "app.h"
#include "ogretest.h"
#include "luamanager.h"

LuaManager luaManager;

static int lscript_test(lua_State* s) {

    int ri;
    const char* rs;
    double rs;
    LUA_NUMBER mat[9];

    luaManager.GetParams("isdm", &ri, &rs, &rs, &mat);

    std::cout << rs << std::endl;
    std::cout << ri << std::endl;

    for (int i = 0; i < 9;++i) {
        std::cout << "mat: " << mat[i] << std::endl;
    }

    return 1;
}

bool App::init()
{
    test_ = new OgreTest();
    test_->init();

    luaManager.LoadScript("data/scripts/app.lua");
    luaManager.RegisterFunction("lscript_test", lscript_test);

    const char *b;
    luaManager.Call("App.init", "d>s", 123.0, &b);

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
