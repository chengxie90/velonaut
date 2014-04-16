#include "gametest.h"

using namespace std;

extern GameTest g_app;

bool GameTest::init(int argc, char *argv[])
{
    graphics_ = new GraphicsTest();
    input_ = new InputTest();
    physics_ = new PhysicsTest();

    graphics_->init();
    input_->init();
    physics_->init();

    /* GAME INIT */
    std::vector<double> sphere_position = vector<double>(3);
    sphere_position[0] = 0;
    sphere_position[1] = 50;
    sphere_position[2] = 0;
    std::vector<double> plane_position = vector<double>(3);
    plane_position[0] = 0;
    plane_position[1] = 0;
    plane_position[2] = 0;
    std::vector<double> default_orientation = vector<double>(4);
    default_orientation[0] = 1;
    default_orientation[1] = 0;
    default_orientation[2] = 0;
    default_orientation[3] = 0;
    std::vector<double> default_scale = vector<double>(3);
    default_scale[0] = 1;
    default_scale[1] = 1;
    default_scale[2] = 1;
    sphereRB_ = PhysicsTest::GetInstance()->createDynamicRigidBody(sphere_position,
                                                                   default_orientation,
                                                                   default_scale,
                                                                   "SPHERE");
    planeRB_ = PhysicsTest::GetInstance()->createStaticRigidBody(plane_position,
                                                                 default_orientation,
                                                                 default_scale,
                                                                 "PLANE");
    sphereSN_ = GraphicsTest::GetInstance()->createSphere(sphere_position, default_orientation, default_scale[0]);
    planeSN_ = GraphicsTest::GetInstance()->createPlane(plane_position, default_orientation);


    return true;
}

void GameTest::run()
{
    while (!terminated_) {
        input_->update();
        physics_->update();

        /* GAME UPDATE */
        std::vector<double> sphere_position = PhysicsTest::GetInstance()->getRigidBodyPosition(sphereRB_);
        std::vector<double> sphere_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(sphereRB_);
        GraphicsTest::GetInstance()->setSceneNodePosition(sphereSN_, sphere_position);
        GraphicsTest::GetInstance()->setSceneNodeOrientation(sphereSN_, sphere_orientation);

        graphics_->render();
    }
}

void GameTest::shutdown()
{
    input_->shutdown();
    graphics_->shutdown();
    physics_->shutdown();

    delete input_;
    delete graphics_;
    delete physics_;
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

