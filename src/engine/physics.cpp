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
            {"setPosition", Physics::RigidBody::lsetPosition},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
}

void Physics::update(float dt)
{
    world_->stepSimulation(dt);
    
    int numManifolds = world_->getDispatcher()->getNumManifolds();
    
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
    double mass;
    LuaManager::GetInstance()->extractParam(&mass);
    
    string shapename;
    LuaManager::GetInstance()->extractParam(&shapename);
    
    btCollisionShape* shape = NULL;
    
    if (shapename == "box") {
        btVector3 extents;
        LuaManager::GetInstance()->extractParam(&extents);
        shape = new btBoxShape(extents);
    }
    else {
        assert(false);
    }
    
    btDefaultMotionState *motionState = new btDefaultMotionState();

    btVector3 inertia;
    shape->calculateLocalInertia(mass, inertia);
    
    btRigidBody *body = new btRigidBody(mass, motionState, shape, inertia);
    
    //body->setCollisionFlags(body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE);
    
    Physics::GetInstance()->world_->addRigidBody(body);
    
    LuaManager::GetInstance()->addParam((void *)body);
    
    return 1;
}

int Physics::RigidBody::ldestroy(lua_State *)
{
    return 0;
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

int Physics::RigidBody::lsetPosition(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);
    
    btVector3 pos;
    LuaManager::GetInstance()->extractParam(&pos);
    
    btTransform trans;
    trans = body->getCenterOfMassTransform();
    
    trans.setOrigin(pos);
    
    body->setCenterOfMassTransform(trans);
    
    btMotionState* ms = body->getMotionState();
    ms->setWorldTransform(trans);
    
    return 0;
}
