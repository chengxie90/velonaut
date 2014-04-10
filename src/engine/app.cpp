#include "app.h"
#include "ogretest.h"
#include "bullettest.h"
#include "luamanager.h"

bool App::init()
{
    test_ = new OgreTest();
    test_->init();
    BulletTest* bulletTest = new BulletTest();
    bulletTest->test();
    LuaManager luaManager;

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
