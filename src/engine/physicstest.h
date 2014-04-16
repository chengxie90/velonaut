#ifndef PHYSICSTEST_H
#define PHYSICSTEST_H

#include <bullet/btBulletDynamicsCommon.h>
#include "common.h"

#include <vector>

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

    btBroadphaseInterface* broadphase_ = NULL;
    btDefaultCollisionConfiguration* collisionConfiguration_ = NULL;
    btCollisionDispatcher* dispatcher_ = NULL;
    btSequentialImpulseConstraintSolver* solver_ = NULL;
    btDynamicsWorld* world_ = NULL;

    void stepWorld(double time = 1/60.0);

    static btRigidBody* createDynamicRigidBody(std::vector<double> position,
                                               std::vector<double> orientation,
                                               std::vector<double> scale,
                                               std::string type);
    static btRigidBody* createStaticRigidBody(std::vector<double> position,
                                              std::vector<double> orientation,
                                              std::vector<double> scale,
                                              std::string type);

    static std::vector<double> getRigidBodyPosition(btRigidBody* node);
    static void setRigidBodyPosition(btRigidBody* node, std::vector<double> position);
    static std::vector<double> getRigidBodyOrientation(btRigidBody* node);
    static void setRigidBodyOrientation(btRigidBody* node, std::vector<double> orientation);
};

#endif // PHYSICSTEST_H
