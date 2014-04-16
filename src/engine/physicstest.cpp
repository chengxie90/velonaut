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
