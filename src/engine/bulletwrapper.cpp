#include "bulletwrapper.h"

static BulletWrapper& BulletWrapper::shared_instance()
{
    static BulletWrapper wrapper;
    return wrapper;
}


BulletWrapper::BulletWrapper() {}

// World stuff
static int CreateDynamicsWorld(lua_State* L)
{

}

static void SetWorldGravity(lua_State* L)
{

}

static void StepSimulation(lua_State* L)
{

}

// Rigid body stuff
static int CreateRigidBody(lua_State* L);
static int GetRigidBodyTransform(lua_State* L);
static int GetRigidBodyPosition(lua_State* L);
static int GetRigidBodyOrientation(lua_State* L);
static void SetRigidBodyTransform(lua_State* L);
static void SetRigidBodyPosition(lua_State* L);
static void SetRigidBodyOrientation(lua_State* L);
static void DeleteRigidBody(lua_State* L);

// Collision shape stuff
static int CreateSphereCollisionShape(lua_State* L);
static int CreateBoxCollisionShape(lua_State* L);
static int CreateCylinderCollisionShape(lua_State* L);
static int CreateCapsuleCollisionShape(lua_State* L);
static int CreateCompoundCollisionShape(lua_State* L);
static void AddChildShapeToCompound(lua_State* L);
static int GetChildShapeByIndex(lua_State* l);
static void RemoveChildShapeByIndex(lua_State* L);
static void GetNumChildShapes(lua_State* L);
static void SetCollisionShapeLocalScaling(lua_State* L);
static void DeleteCollisionShape(lua_State* L);


/*
//-------------------------------------------------------------------------------------------------
//
// World methods

static void BulletWrapper::create_dynamics_world()
{
    collision_configuration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collision_configuration);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
    dynamics_world->setGravity(btVector3(0,-10,0));
}

static void BulletWrapper::reset_dynamics_world()
{
    // free memory
    delete dispatcher;
    delete collision_configuration;
    delete broadphase;
    delete solver;
    delete dynamics_world;
    for (rigid_body_map_iterator it = rigid_bodies.begin(); it != rigid_bodies.end(); it++)
        delete it->second;
    for (collision_object_map_iterator it = collision_objects.begin(); it != collision_objects.end(); it++)
        delete it->second;

    // clear maps
    rigid_bodies_to_collision_objects.clear();
    rigid_bodies.clear(); rigid_bodies_counter = 0;
    collision_objects.clear(); collision_objects_counter = 0;

    // recreate world
    create_dynamics_world();
}

static void BulletWrapper::step_simulation()
{
    dynamics_world->stepSimulation();
}

//-------------------------------------------------------------------------------------------------
//
// Tunnel methods

static void BulletWrapper::create_tunnel(std::list<btVector3> path, double radius, double circle)
{

}

static void BulletWrapper::reset_tunnel(std::list<btVector3> path, double radius, double circle)
{

}

//-------------------------------------------------------------------------------------------------
//
// Rigid body methods

static int BulletWrapper::create_rigid_body(int collision_object_handle)
{
    // create object in Bullet
    btCollisionObject* collision_object = collision_objects[collision_object_handle];
    btDefaultMotionState* motion_state =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            construction_info(0,motion_state,collision_object,btVector3(0,0,0));
    btRigidBody* rigid_body = new btRigidBody(construction_info);

    // add object to maps and then to Bullet world
    rigid_bodies[rigid_bodies_counter++];
    rigid_bodies_to_collision_objects[rigid_body] = collision_objects[collision_object_handle];
    dynamics_world->addRigidBody(rigid_body);
}

static double* BulletWrapper::get_transform(int handle) const
{
    btRigidBody* rigid_body = rigid_bodies[handle];
    btScalar transform[9];
    rigid_body->getWorldTransform().getOpenGLMatrix(transform);
    btVector3 scaling = rigid_body->getCollisionShape()->getLocalScaling();
}

static double* BulletWrapper::get_position(int handle);
static double BulletWrapper::get_oriantation(int handle);
static void BulletWrapper::set_transform(int handle, double* transform);
static void BulletWrapper::set_position(int handle, double* position);
static void BulletWrapper::set_orientation(int handle, double* orientation);
static bool BulletWrapper::delete_rigid_body(int handle);

//-------------------------------------------------------------------------------------------------
//
// Collision object methods

static int BulletWrapper::create_sphere_shape(double radius);
static int BulletWrapper::create_box_shape(double x, double y, double z);
static int BulletWrapper::create_cylinder_shape(double radius, double height);
static int BulletWrapper::create_triangle_mesh_shape(std::string filename);
static void BulletWrapper::set_shape_scaling(int handle, double x_scale, double y_scale, double z_scale);
static bool BulletWrapper::delete_shape(int handle);
*/
