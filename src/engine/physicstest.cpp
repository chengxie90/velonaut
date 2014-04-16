#include "physicstest.h"
#include "gametest.h"

using namespace std;

PhysicsTest::PhysicsTest() {}

void PhysicsTest::init()
{
    broadphase_ = new btDbvtBroadphase();
    assert(broadphase_);
    collisionConfiguration_ = new btDefaultCollisionConfiguration();
    assert(collisionConfiguration_);
    solver_ = new btSequentialImpulseConstraintSolver;
    assert(solver_);
    dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
    assert(dispatcher_);
    world_ = new btDiscreteDynamicsWorld(PhysicsTest::GetInstance()->dispatcher_,
                                         PhysicsTest::GetInstance()->broadphase_,
                                         PhysicsTest::GetInstance()->solver_,
                                         PhysicsTest::GetInstance()->collisionConfiguration_);
    assert(world_);
}

void PhysicsTest::update()
{
    stepWorld();
}

void PhysicsTest::shutdown()
{
    delete broadphase_;
    delete collisionConfiguration_;
    delete dispatcher_;
    delete solver_;
    delete world_;
}

PhysicsTest* PhysicsTest::GetInstance()
{
    return GameTest::GetApp()->GetPhysics();
}

void PhysicsTest::stepWorld(double time)
{
    PhysicsTest::GetInstance()->world_->stepSimulation(time);
}

btRigidBody* PhysicsTest::createDynamicRigidBody(std::vector<double> position,
                                                 std::vector<double> orientation,
                                                 std::vector<double> scale,
                                                 std::string type)
{
    if (type == "SPHERE")
    {
        btCollisionShape* sphere = new btSphereShape(scale[0]);
        btTransform transform = btTransform(
                    btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]),
                    btVector3(position[0], position[1], position[2]));
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        btScalar mass = 1;
        btVector3 inertia(0,0,0);
        sphere->calculateLocalInertia(mass,inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,sphere,inertia);
        btRigidBody* sphereBody = new btRigidBody(rigidBodyCI);
        PhysicsTest::GetInstance()->world_->addRigidBody(sphereBody);
        return sphereBody;
    }
    return NULL;
}

btRigidBody* PhysicsTest::createStaticRigidBody(std::vector<double> position,
                                                std::vector<double> orientation,
                                                std::vector<double> scale,
                                                std::string type)
{
    if (type == "PLANE")
    {
        btCollisionShape* plane = new btStaticPlaneShape(btVector3(0,1,0),1);
        btTransform transform = btTransform(
                    btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]),
                    btVector3(position[0], position[1], position[2]));
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo
                rigidBodyCI(0,motionState,plane,btVector3(0,0,0));
        btRigidBody* planeBody = new btRigidBody(rigidBodyCI);
        PhysicsTest::GetInstance()->world_->addRigidBody(planeBody);
        return planeBody;
    }
    return NULL;
}

std::vector<double> PhysicsTest::getRigidBodyPosition(btRigidBody* node)
{
    std::vector<double> ret = std::vector<double>(3);
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    btVector3 position = transform.getOrigin();
    ret[0] = position.x();
    ret[1] = position.y();
    ret[2] = position.z();
    return ret;
}

void PhysicsTest::setRigidBodyPosition(btRigidBody* node, std::vector<double> position)
{
    btVector3 origin = btVector3(position[0], position[1], position[2]);
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(origin);
    node->getMotionState()->setWorldTransform(transform);
}

std::vector<double> PhysicsTest::getRigidBodyOrientation(btRigidBody* node)
{
    std::vector<double> ret = std::vector<double>(4);
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    btQuaternion orientation = transform.getRotation();
    ret[0] = orientation.w();
    ret[1] = orientation.x();
    ret[2] = orientation.y();
    ret[3] = orientation.z();
    return ret;
}

void PhysicsTest::setRigidBodyOrientation(btRigidBody* node, std::vector<double> orientation)
{
    btQuaternion origin = btQuaternion(orientation[0], orientation[1], orientation[2], orientation[3]);
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    transform.setRotation(origin);
    node->getMotionState()->setWorldTransform(transform);
}
