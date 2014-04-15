#include "app.h"
#include "ogretest.h"
#include "luamanager.h"

LuaManager luaManager;

static int lscript_bla(lua_State* s) {

    int ri;
    const char* ss;
    double d;
    luaManager.GetParams("isd", &ri, &ss, &d);

    std::cout << ss << std::endl;
    std::cout << ri << std::endl;
    std::cout << d << std::endl;

    return 1;
}

bool App::init()
{
    test_ = new OgreTest();
    test_->init();

    luaManager.LoadScript("data/scripts/app.lua");
    luaManager.RegisterFunction("lscript_bla", lscript_bla);

    LUA_NUMBER m[9];
    m[0] = 0;
    m[1] = 1;
    m[2] = 2;

    double d1;
    char s[1024];
    int i;
    luaManager.Call("hello", "dmdsi>dsi", 20.2, m, 23.3, "50 cent", 30,
                                        &d1, &s, &i);
    std::cout << d1 << std::endl;
    std::cout << s << std::endl;
    std::cout << i << std::endl;

    luaManager.Call("App.init");

    /*
    LUA_NUMBER m[3];
    m[0] = 0;
    m[1] = 1;
    m[2] = 2;

    LUA_NUMBER returnedMatrix[9];
    double d;
    double d1;
    luaManager.Call("hello", "dmd>m", 1.0, m, 2.0, &returnedMatrix);
    for (int i = 0; i < 9;++i) {
        std::cout << "ma: " << returnedMatrix[i] << std::endl;
    }

    std::cout << d << std::endl;
    std::cout << d1 << std::endl;
    */
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
