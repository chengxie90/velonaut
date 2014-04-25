#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.h"
#include "btBulletDynamicsCommon.h"
#include "luamanager.h"

class Physics
{
public:
    void init();
    void initLua();
    void update(float dt);
    void shutdown();
    
    static Physics* GetInstance();
    
private:
    friend class App;
    SINGLETON(Physics)
    
    btDiscreteDynamicsWorld* world_;
    btDbvtBroadphase* overlappingPairCache_;
    btCollisionDispatcher* dispatcher_;
    btDefaultCollisionConfiguration* collisionConfiguration_;
    btSequentialImpulseConstraintSolver* solver_;    
    
    struct RigidBody {
        static int lcreate(lua_State *);
        static int ldestroy(lua_State *);
        
        static int lsetTrigger(lua_State *);
        
        static int lposition(lua_State *);
        static int lorientation(lua_State *);
        static int llinearVelocity(lua_State *);
        static int langularVelocity(lua_State *);
        static int lforce(lua_State *);
        static int ltorque(lua_State *);

        static int lsetPosition(lua_State *);
        static int lsetOrientation(lua_State *);
        static int lsetLinearVelocity(lua_State *);
        static int lsetAngularVelocity(lua_State *);
        static int lsetDamping(lua_State *);

        static int lapplyCentralForce(lua_State *);
        static int lapplyForce(lua_State *);
        static int lapplyTorque(lua_State *);
        static int lclearForces(lua_State *);
    };
};

#endif // PHYSICS_H
