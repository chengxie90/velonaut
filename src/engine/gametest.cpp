#include "gametest.h"
#include "graphicstest.h"
#include "inputtest.h"
#include "physicstest.h"
#include "luamanager.h"

using namespace std;

extern GameTest g_app;

bool GameTest::init(int argc, char *argv[])
{
    graphics_ = new GraphicsTest();
    input_ = new InputTest();

    graphics_->init();
    input_->init();


    return true;
}

void GameTest::run()
{
    while (!terminated_) {
        input_->update();
        graphics_->render();
    }
}

void GameTest::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();

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

GraphicsTest *GameTest::GetGraphics()
{
    return g_app.graphics_;
}

InputTest *GameTest::GetInput()
{
    return g_app.input_;
}

PhysicsTest *GameTest::GetPhysics()
{
    return g_app.physics_;
}

