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

void PhysicsTest::setGravity(std::vector<double> gravity)
{
    PhysicsTest::GetInstance()->world_->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));
}

btRigidBody* PhysicsTest::createDynamicRigidBody(std::vector<double> position,
                                                 std::vector<double> orientation,
                                                 std::vector<double> scale,
                                                 std::string type)
{
    if (type == "SPHERE")
    {
        btCollisionShape* sphere = new btSphereShape(scale[0]/2);
        btTransform transform = btTransform(
                    btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]),
                    btVector3(position[0], position[1], position[2]));
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        btScalar mass = 1;
        btVector3 inertia(0, 0, 0);
        sphere->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, sphere, inertia);
        btRigidBody* sphereBody = new btRigidBody(rigidBodyCI);
        sphereBody->setFriction(1000);
        PhysicsTest::GetInstance()->world_->addRigidBody(sphereBody);
        return sphereBody;
    }
    if (type == "CUBE")
    {
        btCollisionShape* cube = new btBoxShape(btVector3(scale[0]/2, scale[1]/2, scale[2]/2));
        btTransform transform = btTransform(
                    btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]),
                    btVector3(position[0], position[1], position[2]));
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        btScalar mass = 1;
        btVector3 inertia(0, 0, 0);
        cube->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, cube, inertia);
        btRigidBody* cubeBody = new btRigidBody(rigidBodyCI);
        PhysicsTest::GetInstance()->world_->addRigidBody(cubeBody);
        return cubeBody;
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

    if (type == "CUBE")
    {
        btCollisionShape* cube = new btBoxShape(btVector3(scale[0]/2, scale[1]/2, scale[2]/2));
        btTransform transform = btTransform(
                    btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]),
                    btVector3(position[0], position[1], position[2]));
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        btScalar mass = 0;
        btVector3 inertia(0, 0, 0);
        cube->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, cube, inertia);
        btRigidBody* cubeBody = new btRigidBody(rigidBodyCI);
        PhysicsTest::GetInstance()->world_->addRigidBody(cubeBody);
        cubeBody->setGravity(btVector3(0,0,0));
        return cubeBody;
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

void PhysicsTest::setRigidBodyAngularVelocity(btRigidBody* node, std::vector<double> roll)
{
    node->setAngularVelocity(btVector3(roll[0], roll[1], roll[2]));
}

void PhysicsTest::addRigidBodyAngularVelocity(btRigidBody* node, std::vector<double> roll)
{
    btVector3 addAV = btVector3(roll[0], roll[1], roll[2]);
    btVector3 curAV = node->getAngularVelocity();
    addAV *= 0.51;
    curAV *= 0.49;
    addAV += curAV;
    node->setAngularVelocity(addAV);
}

std::vector<double> PhysicsTest::getCameraPos(btRigidBody* ship, double camDis)
{
    btTransform transform;
    ship->getMotionState()->getWorldTransform(transform);
    btVector3 position = transform.getOrigin();
    btQuaternion orientation = transform.getRotation();
    btVector3 look = btVector3(0,0,-1);
    look = look.rotate(orientation.getAxis(), orientation.getAngle());
    look.normalize(); look *= (-1 * camDis);
    position += look;
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = position.x();
    ret[1] = position.y();
    ret[2] = position.z();
    return ret;
}

void PhysicsTest::applyRigidBodyTorque(btRigidBody* node, std::vector<double> torque, std::vector<double> maxAngularVelocity)
{
    btVector3 currAngularVel = node->getAngularVelocity();
    btVector3 torqueVec = btVector3(torque[0], torque[1], torque[2]);
    node->activate(true);
    node->applyTorque(torqueVec);
}

void PhysicsTest::applyRigidBodyForce(btRigidBody* node, std::vector<double> force, std::vector<double> maxVelocity)
{
    btVector3 forceVec = btVector3(force[0], force[1], force[2]);
    node->activate(true);
    node->applyCentralForce(forceVec);
}

std::vector<double> PhysicsTest::getRigidBodyLook(btRigidBody* node)
{
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    btQuaternion orientation = transform.getRotation();
    btVector3 look = btVector3(0,0,-1);
    look = look.rotate(orientation.getAxis(), orientation.getAngle());
    look.normalize();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = look.x();
    ret[1] = look.y();
    ret[2] = look.z();
    return ret;
}

std::vector<double> PhysicsTest::getRigidBodyUp(btRigidBody* node)
{
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    btQuaternion orientation = transform.getRotation();
    btVector3 look = btVector3(0,1,0);
    look = look.rotate(orientation.getAxis(), orientation.getAngle());
    look.normalize();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = look.x();
    ret[1] = look.y();
    ret[2] = look.z();
    return ret;
}

std::vector<double> PhysicsTest::getRigidBodyRight(btRigidBody* node)
{
    btTransform transform;
    node->getMotionState()->getWorldTransform(transform);
    btQuaternion orientation = transform.getRotation();
    btVector3 look = btVector3(1,0,0);
    look = look.rotate(orientation.getAxis(), orientation.getAngle());
    look.normalize();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = look.x();
    ret[1] = look.y();
    ret[2] = look.z();
    return ret;
}

btVector3 PhysicsTest::getRigidBodyVelocity(btRigidBody* node)
{
    return node->getLinearVelocity();
}

void PhysicsTest::setRigidBodyVelocity(btRigidBody* node, btVector3 velocity)
{
    node->setLinearVelocity(velocity);
}

void PhysicsTest::setDamping(btRigidBody* node, double linearDamping, double angularDamping)
{
    node->setDamping(linearDamping, angularDamping);
}
