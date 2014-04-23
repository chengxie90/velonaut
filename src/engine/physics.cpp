#include "physics.h"
#include "app.h"

using namespace std;

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
    
    // TODO: 
    world_->setGravity(btVector3(0, -10, 0));
}

void Physics::initLua()
{
    LuaManager::GetInstance()->requiref("engine.physics.rigidbody.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Physics::RigidBody::lcreate},
            {"position", Physics::RigidBody::lposition},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
}

void Physics::update(float dt)
{
    world_->stepSimulation(dt);
    
//    int num = world_->getNumCollisionObjects();
    
//    btRigidBody* body = btRigidBody::upcast(world_->getCollisionObjectArray()[0]);

//    btTransform trans;
//    body->getMotionState()->getWorldTransform(trans);
    
//    btVector3 pos = trans.getOrigin();
    
//    cout << pos.y() << endl;
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

int Physics::RigidBody::lcreate(lua_State *)
{
    btDefaultMotionState *motionState = new btDefaultMotionState();
    btEmptyShape* sprShape = new btEmptyShape;
    btRigidBody *body = new btRigidBody(1, motionState, sprShape);
    Physics::GetInstance()->world_->addRigidBody(body);
    
    LuaManager::GetInstance()->addParam((void *)body);
    
    return 1;
}

int Physics::RigidBody::lsetShape(lua_State *)
{
//    string shape;
//    LuaManager::GetInstance()->extractParam(&shape);
    
//    btCollisionShape* shape = NULL;
    
//    if (shape == "box") {
//        shape = btBoxShape({1, 1, 1});
//    }
//    else if (shape == "sphere") {
//        shape = btSphereShape(1);
//    }
//    else {
//        assert(false);
//    }
    
}

int Physics::RigidBody::lposition(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);
    
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    
    btVector3 pos = trans.getOrigin();
    
    LuaManager::GetInstance()->addParam(pos);
    
    return 1;
}
