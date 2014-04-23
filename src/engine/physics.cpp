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
    world_->setGravity(btVector3(0, 0, 0));
}

void Physics::initLua()
{
    LuaManager::GetInstance()->requiref("engine.physics.rigidbody.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Physics::RigidBody::lcreate},
            {"position", Physics::RigidBody::lposition},
            {"position", Physics::RigidBody::lposition},
            {"setPosition", Physics::RigidBody::lsetPosition},
            {"orientation", Physics::RigidBody::lorientation},
            {"setOrientation", Physics::RigidBody::lsetOrientation},
            {"orientation", Physics::RigidBody::lorientation},
            {"applyCentralForce", Physics::RigidBody::lapplyCentralForce},
            {"applyForce", Physics::RigidBody::lapplyForce},
            {"applyTorque", Physics::RigidBody::lapplyTorque},
            {"setLinearVelocity", Physics::RigidBody::lsetLinearVelocity},
            {"setAngularVelocity", Physics::RigidBody::lsetAngularVelocity},
            {"setDamping", Physics::RigidBody::lsetDamping},
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

int Physics::RigidBody::lsetPosition(lua_State *)
{
    btRigidBody *body;
    btVector3 position;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&position);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(position);
    body->getMotionState()->setWorldTransform(trans);

    return 0;
}

int Physics::RigidBody::lorientation(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);

    btQuaternion ori = trans.getRotation();

    LuaManager::GetInstance()->addParam(ori);

    return 1;
}

int Physics::RigidBody::lsetOrientation(lua_State *)
{
    btRigidBody *body;
    btQuaternion orientation;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&orientation);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setRotation(orientation);
    body->getMotionState()->setWorldTransform(trans);

    return 0;
}

int Physics::RigidBody::lapplyCentralForce(lua_State *)
{
    btRigidBody *body;
    btVector3 force;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&force);

    body->applyCentralForce(force);

    return 0;
}

int Physics::RigidBody::lapplyForce(lua_State *)
{
    btRigidBody *body;
    btVector3 force;
    btVector3 relativePosition;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&force);
    LuaManager::GetInstance()->extractParam(&relativePosition);

    body->applyForce(force, relativePosition);

    return 0;
}

int Physics::RigidBody::lapplyTorque(lua_State *)
{
    btRigidBody *body;
    btVector3 torque;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&torque);

    body->applyTorque(torque);

    return 0;
}

int Physics::RigidBody::lsetLinearVelocity(lua_State *)
{
    btRigidBody *body;
    btVector3 velocity;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&velocity);

    body->setLinearVelocity(velocity);

    return 0;
}

int Physics::RigidBody::lsetAngularVelocity(lua_State *)
{
    btRigidBody *body;
    btVector3 velocity;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&velocity);

    body->setAngularVelocity(velocity);

    return 0;
}

int Physics::RigidBody::lsetDamping(lua_State *)
{
    btRigidBody *body;
    double linearDamping;
    double angularDamping;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&linearDamping);
    LuaManager::GetInstance()->extractParam(&angularDamping);

    body->setDamping(linearDamping, angularDamping);

    return 0;
}
