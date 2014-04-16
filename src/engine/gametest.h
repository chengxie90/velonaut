#ifndef GAMETEST_H
#define GAMETEST_H

#include "graphicstest.h"
#include "inputtest.h"
#include "physicstest.h"
#include "common.h"

class GraphicsTest;
class InputTest;
class PhysicsTest;

class GameTest {
public:
    bool init(int argc, char *argv[]);
    void run();
    void shutdown();
    void terminate();

    static GameTest* GetApp();
    static GraphicsTest* GetGraphics();
    static InputTest* GetInput();
    static PhysicsTest* GetPhysics();

private:
    bool terminated_ = false;

    btRigidBody* cubeRB_ = NULL;
    btRigidBody* sphereRB_ = NULL;
    btRigidBody* planeRB_ = NULL;

    Ogre::SceneNode* sphereSN_ = NULL;
    Ogre::SceneNode* planeSN_ = NULL;
    Ogre::SceneNode* cubeSN_ = NULL;

    GraphicsTest* graphics_ = NULL;
    InputTest* input_ = NULL;
    PhysicsTest* physics_ = NULL;
};

#endif // GAMETEST_H
