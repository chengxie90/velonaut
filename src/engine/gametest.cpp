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
    sphere_position[0] = 0.5;
    sphere_position[1] = 1.5;
    sphere_position[2] = 0;
    std::vector<double> cube_position = vector<double>(3);
    cube_position[0] = 0;
    cube_position[1] = 10;
    cube_position[2] = 0;
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
    std::vector<double> plane_scale = vector<double>(3);
    plane_scale[0] = 100;
    plane_scale[1] = 0.01;
    plane_scale[2] = 100;
    sphereRB_ = PhysicsTest::GetInstance()->createDynamicRigidBody(sphere_position,
                                                                   default_orientation,
                                                                   default_scale,
                                                                   "SPHERE");

    cubeRB_ = PhysicsTest::GetInstance()->createDynamicRigidBody(cube_position,
                                                                 default_orientation,
                                                                 default_scale,
                                                                 "CUBE");
    planeRB_ = PhysicsTest::GetInstance()->createStaticRigidBody(plane_position,
                                                                 default_orientation,
                                                                 plane_scale,
                                                                 "CUBE");
    sphereSN_ = GraphicsTest::GetInstance()->createEllipsoid(sphere_position, default_orientation, default_scale);
    cubeSN_ = GraphicsTest::GetInstance()->createBox(cube_position, default_orientation, default_scale);
    planeSN_ = GraphicsTest::GetInstance()->createBox(plane_position, default_orientation, plane_scale);


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
        std::vector<double> cube_position = PhysicsTest::GetInstance()->getRigidBodyPosition(cubeRB_);
        std::vector<double> cube_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(cubeRB_);
        std::vector<double> plane_position = PhysicsTest::GetInstance()->getRigidBodyPosition(planeRB_);
        std::vector<double> plane_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(planeRB_);
        GraphicsTest::GetInstance()->setSceneNodePosition(cubeSN_, cube_position);
        GraphicsTest::GetInstance()->setSceneNodeOrientation(cubeSN_, cube_orientation);
        GraphicsTest::GetInstance()->setSceneNodePosition(sphereSN_, sphere_position);
        GraphicsTest::GetInstance()->setSceneNodeOrientation(sphereSN_, sphere_orientation);
        GraphicsTest::GetInstance()->setSceneNodePosition(planeSN_, plane_position);
        GraphicsTest::GetInstance()->setSceneNodeOrientation(planeSN_, plane_orientation);

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

