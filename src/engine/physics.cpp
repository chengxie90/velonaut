#include "physics.h"
#include "app.h"

Physics::Physics()
{
}

void Physics::init()
{
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
	overlappingPairCache_ = new btDbvtBroadphase();
	solver_ = new btSequentialImpulseConstraintSolver;
	world_ = new btDiscreteDynamicsWorld(dispatcher_, overlappingPairCache_, solver_, collisionConfiguration_);
    
    // TODO: remove
    world_->setGravity(btVector3(0, -10, 0));
    
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
}

void Physics::update(float dt)
{
    
}

void Physics::shutdown()
{
	delete world_;
	delete solver_;
	delete overlappingPairCache_;
	delete dispatcher_;
	delete collisionConfiguration_;
}

Physics *Physics::GetInstance()
{
    return App::GetApp()->GetPhysics();
}
