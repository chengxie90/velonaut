#ifndef PHYSICSTEST_H
#define PHYSICSTEST_H

#include <bullet/btBulletDynamicsCommon.h>
#include "common.h"

class PhysicsTest
{
public:
    void init();
    void update();
    void shutdown();

    static PhysicsTest* GetInstance();

private:
    SINGLETON(PhysicsTest)
    friend class GameTest;

    // tmp
    void createWorld();

    btBroadphaseInterface* broadphase_ = NULL;
    btDefaultCollisionConfiguration* collisionConfiguration_ = NULL;
    btCollisionDispatcher* dispatcher_ = NULL;
    btSequentialImpulseConstraintSolver* solver_ = NULL;
    btDynamicsWorld* world_ = NULL;
};

#endif // PHYSICSTEST_H
