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
            {"setTrigger", Physics::RigidBody::lsetTrigger},
            {"position", Physics::RigidBody::lposition},
            {"orientation", Physics::RigidBody::lorientation},
            {"linearVelocity", Physics::RigidBody::llinearVelocity},
            {"angularVelocity", Physics::RigidBody::langularVelocity},
            {"force", Physics::RigidBody::lforce},
            {"torque", Physics::RigidBody::ltorque},

            {"setPosition", Physics::RigidBody::lsetPosition},
            {"setOrientation", Physics::RigidBody::lsetOrientation},
            {"setLinearVelocity", Physics::RigidBody::lsetLinearVelocity},
            {"setAngularVelocity", Physics::RigidBody::lsetAngularVelocity},
            {"setDamping", Physics::RigidBody::lsetDamping},

            {"applyCentralForce", Physics::RigidBody::lapplyCentralForce},
            {"applyForce", Physics::RigidBody::lapplyForce},
            {"applyTorque", Physics::RigidBody::lapplyTorque},
            {"clearForces", Physics::RigidBody::lclearForces},
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

    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold =  world_->getDispatcher()->getManifoldByIndexInternal(i);
        const btRigidBody* obA = static_cast<const btRigidBody*>(contactManifold->getBody0());
        const btRigidBody* obB = static_cast<const btRigidBody*>(contactManifold->getBody1());
        
        btVector3 p = {0, 0, 0};
        int numContacts = contactManifold->getNumContacts();
        int count = 0;
        for (int j = 0; j < numContacts; j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                p += ptA;
                count += 1;
//                const btVector3& ptB = pt.getPositionWorldOnB();
//                const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }

        p /= count;
        
        LuaManager::GetInstance()->addFunction("RigidBody._onGlobalCollision");
        LuaManager::GetInstance()->addParamReg((void *)obA);
        LuaManager::GetInstance()->addParamReg((void *)obB);
        LuaManager::GetInstance()->addParam(p);
        LuaManager::GetInstance()->pCall(3);
    }
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
        
    Physics::GetInstance()->world_->addRigidBody(body);
    
    LuaManager::GetInstance()->addParam((void *)body);
    
    LuaManager::GetInstance()->setReg((void *)body);
    
    return 1;
}

int Physics::RigidBody::ldestroy(lua_State *)
{
    return 0;
}

int Physics::RigidBody::lsetTrigger(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);
    
    bool trigger;
    LuaManager::GetInstance()->extractParam(&trigger);
    
    if (trigger) {
        body->setCollisionFlags(body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE);
    }
    else {
        body->setCollisionFlags(body->getCollisionFlags() & ~btRigidBody::CF_NO_CONTACT_RESPONSE);
    }
    
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

int Physics::RigidBody::llinearVelocity(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btVector3 vel = body->getLinearVelocity();

    LuaManager::GetInstance()->addParam(vel);

    return 1;
}

int Physics::RigidBody::langularVelocity(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btVector3 vel = body->getAngularVelocity();

    LuaManager::GetInstance()->addParam(vel);

    return 1;
}

int Physics::RigidBody::lforce(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btVector3 force = body->getTotalForce();

    LuaManager::GetInstance()->addParam(force);

    return 1;
}

int Physics::RigidBody::ltorque(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btVector3 torque = body->getTotalTorque();

    LuaManager::GetInstance()->addParam(torque);

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


int Physics::RigidBody::lsetOrientation(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    btQuaternion orientation;
    LuaManager::GetInstance()->extractParam(&orientation);

    btTransform trans;
    trans = body->getCenterOfMassTransform();

    trans.setRotation(orientation);

    body->setCenterOfMassTransform(trans);

    btMotionState* ms = body->getMotionState();
    ms->setWorldTransform(trans);

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

int Physics::RigidBody::lapplyCentralForce(lua_State *)
{
    btRigidBody *body;
    btVector3 force;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&force);

    body->activate(true);
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

    body->activate(true);
    body->applyForce(force, relativePosition);

    return 0;
}

int Physics::RigidBody::lapplyTorque(lua_State *)
{
    btRigidBody *body;
    btVector3 torque;
    LuaManager::GetInstance()->extractParam((void **)&body);
    LuaManager::GetInstance()->extractParam(&torque);

    body->activate(true);
    body->applyTorque(torque);

    return 0;
}

int Physics::RigidBody::lclearForces(lua_State *)
{
    btRigidBody *body;
    LuaManager::GetInstance()->extractParam((void **)&body);

    body->clearForces();

    return 0;
}

