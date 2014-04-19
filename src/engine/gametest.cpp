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
//    std::vector<double> sphere_position = vector<double>(3);
//    sphere_position[0] = 0.5;
//    sphere_position[1] = 1.5;
//    sphere_position[2] = 0;
    std::vector<double> cube_position = vector<double>(3);
    cube_position[0] = 0; cube_position[1] = 0; cube_position[2] = 0;
//    std::vector<double> plane_position = vector<double>(3);
//    plane_position[0] = 0;
//    plane_position[1] = 0;
//    plane_position[2] = 0;
    std::vector<double> default_orientation = vector<double>(4);
    default_orientation[0] = 1; default_orientation[1] = 0; default_orientation[2] = 0; default_orientation[3] = 0;
    std::vector<double> default_scale = vector<double>(3);
    default_scale[0] = 0.05; default_scale[1] = 0.01; default_scale[2] = 0.05;
//    std::vector<double> plane_scale = vector<double>(3);
//    plane_scale[0] = 100;
//    plane_scale[1] = 0.01;
//    plane_scale[2] = 100;
//    sphereRB_ = PhysicsTest::GetInstance()->createDynamicRigidBody(sphere_position,
//                                                                   default_orientation,
//                                                                   default_scale,
//                                                                   "SPHERE");

    cubeRB_ = PhysicsTest::GetInstance()->createDynamicRigidBody(cube_position,
                                                                 default_orientation,
                                                                 default_scale,
                                                                 "CUBE");
    std::vector<double> grav = std::vector<double>(3);
    PhysicsTest::GetInstance()->setGravity(grav);
//    planeRB_ = PhysicsTest::GetInstance()->createStaticRigidBody(plane_position,
//                                                                 default_orientation,
//                                                                 plane_scale,
//                                                                 "CUBE");
//    sphereSN_ = GraphicsTest::GetInstance()->createEllipsoid(sphere_position, default_orientation, default_scale);
    cubeSN_ = GraphicsTest::GetInstance()->createBox(cube_position, default_orientation, default_scale);
//    planeSN_ = GraphicsTest::GetInstance()->createBox(plane_position, default_orientation, plane_scale);

    PhysicsTest::GetInstance()->setDamping(cubeRB_, 0.9, 1);
    GraphicsTest::createTunnel(1);

    return true;
}

void GameTest::run()
{
    while (!terminated_) {
        input_->update();
        physics_->update();

        /* GAME UPDATE */
//        std::vector<double> sphere_position = PhysicsTest::GetInstance()->getRigidBodyPosition(sphereRB_);
//        std::vector<double> sphere_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(sphereRB_);
        std::vector<double> cube_position = PhysicsTest::GetInstance()->getRigidBodyPosition(cubeRB_);
        std::vector<double> cube_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(cubeRB_);
        btVector3 playerVel = PhysicsTest::GetInstance()->getRigidBodyVelocity(cubeRB_);
        std::vector<double> playerLook = PhysicsTest::GetInstance()->getRigidBodyLook(cubeRB_);
        btVector3 playerLookVec = btVector3(playerLook[0], playerLook[1], playerLook[2]);
        //double speed = playerVel.length();
        //playerVel = speed*playerLookVec.normalized();
        //PhysicsTest::GetInstance()->setRigidBodyVelocity(cubeRB_, playerVel);
//        std::vector<double> plane_position = PhysicsTest::GetInstance()->getRigidBodyPosition(planeRB_);
//        std::vector<double> plane_orientation = PhysicsTest::GetInstance()->getRigidBodyOrientation(planeRB_);
        GraphicsTest::GetInstance()->setSceneNodePosition(cubeSN_, cube_position);
        GraphicsTest::GetInstance()->setSceneNodeOrientation(cubeSN_, cube_orientation);
        std::vector<double> camPos = PhysicsTest::GetInstance()->getCameraPos(cubeRB_, 2);
        std::vector<double> camOri = PhysicsTest::GetInstance()->getRigidBodyOrientation(cubeRB_);
        GraphicsTest::GetInstance()->setCameraPostion(camPos);
        GraphicsTest::GetInstance()->setCameraOrientation(camOri);

//        GraphicsTest::GetInstance()->setSceneNodePosition(sphereSN_, sphere_position);
//        GraphicsTest::GetInstance()->setSceneNodeOrientation(sphereSN_, sphere_orientation);
//        GraphicsTest::GetInstance()->setSceneNodePosition(planeSN_, plane_position);
//        GraphicsTest::GetInstance()->setSceneNodeOrientation(planeSN_, plane_orientation);

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

void GameTest::applyPlayerForce(std::vector<double> force)
{
    std::vector<double> maxVel = std::vector<double>(3);
    maxVel[0] = 0; maxVel[1] = 0; maxVel[2] = 0;
    PhysicsTest::GetInstance()->applyRigidBodyForce(GameTest::GetApp()->cubeRB_, force, maxVel);
}

void GameTest::applyPlayerTorque(std::vector<double> torque)
{
    std::vector<double> maxVel = std::vector<double>(3);
    maxVel[0] = 0; maxVel[1] = 0; maxVel[2] = 0;
    PhysicsTest::GetInstance()->applyRigidBodyTorque(GameTest::GetApp( )->cubeRB_, torque, maxVel);
}

std::vector<double> GameTest::getPlayerLook()
{
    return PhysicsTest::GetInstance()->getRigidBodyLook(GameTest::GetApp()->cubeRB_);
}

std::vector<double> GameTest::getPlayerUp()
{
    return PhysicsTest::GetInstance()->getRigidBodyUp(GameTest::GetApp()->cubeRB_);
}

std::vector<double> GameTest::getPlayerRight()
{
    return PhysicsTest::GetInstance()->getRigidBodyRight(GameTest::GetApp()->cubeRB_);
}

void GameTest::rollPlayer(std::vector<double> roll)
{
    PhysicsTest::GetInstance()->addRigidBodyAngularVelocity(GameTest::GetApp()->cubeRB_, roll);
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

