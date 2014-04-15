#include "bullettest.h"

BulletTest::BulletTest()
{
}

void BulletTest::test()
{
    std::cout << "Hello world from bullet test" << std::endl;

    // Specify our broadphase algorithm
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // Set up out collision configuration for fine-tuned
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // Register our GImpact collision algorithm with our dispatcher
    //btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

    // Create a solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // Finally, creat our dynamics world
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    // Set gravity
    dynamicsWorld->setGravity(btVector3(0,-10,0));


    // Create ground plane
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btCollisionShape* fallShape = new btSphereShape(1);

    btDefaultMotionState* groundMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

    // Create falling ball
    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);

    // Delete
    for (int i=0; i<300; i++) {
        dynamicsWorld->stepSimulation(1/60.f,10);

        btTransform trans;
        fallRigidBody->getMotionState()->getWorldTransform(trans);

        std::cout << trans.getOrigin().getY() << ", " << std::endl;
    }

    // Delete objects
    dynamicsWorld->removeRigidBody(fallRigidBody);
    delete fallRigidBody->getMotionState();
    delete fallRigidBody;

    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;


    // Delete world stuff
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete broadphase;


}
