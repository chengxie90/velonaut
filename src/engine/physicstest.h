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
    friend class InputTest;

    btBroadphaseInterface* broadphase_ = NULL;
    btDefaultCollisionConfiguration* collisionConfiguration_ = NULL;
    btCollisionDispatcher* dispatcher_ = NULL;
    btSequentialImpulseConstraintSolver* solver_ = NULL;
    btDynamicsWorld* world_ = NULL;

    void stepWorld(double time = 1/60.0);
    static void setGravity(std::vector<double> gravity);

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
    static std::vector<double> getRigidBodyLook(btRigidBody* node);
    static std::vector<double> getRigidBodyUp(btRigidBody* node);
    static std::vector<double> getRigidBodyRight(btRigidBody* node);
    static btVector3 getRigidBodyVelocity(btRigidBody* node);
    static void setRigidBodyVelocity(btRigidBody* node, btVector3 velocity);
    void setDamping(btRigidBody* node, double linearDamping, double angularDamping);

    void setRigidBodyAngularVelocity(btRigidBody* node, std::vector<double> roll);
    void addRigidBodyAngularVelocity(btRigidBody* node, std::vector<double> roll);
    void applyRigidBodyTorque(btRigidBody* node, std::vector<double> torque, std::vector<double> maxAngularVelocity);
    void applyRigidBodyForce(btRigidBody* node, std::vector<double> force, std::vector<double> maxVelocity);

    std::vector<double> getCameraPos(btRigidBody* ship, double camDis);
};

#endif // PHYSICSTEST_H
