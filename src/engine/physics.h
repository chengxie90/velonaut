#ifndef PHYSICS_H
#define PHYSICS_H


#include "common.h"
#include "btBulletDynamicsCommon.h"


class Physics
{
public:
    void init();
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
};

#endif // PHYSICS_H
