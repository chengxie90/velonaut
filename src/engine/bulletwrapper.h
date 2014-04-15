#ifndef BULLETWRAPPER_H
#define BULLETWRAPPER_H

#include <btBulletDynamicsCommon.h>
#include <lua/lua.hpp>

#include <iostream>
#include <map>
#include <vector>

#include "src/engine/luamanager.h"

/*
typedef std::map<int, btRigidBody*> rigid_body_map;
typedef std::map<int, btCollisionObject*> collision_object_map;
typedef std::map<int, btRigidBody*>::iterator rigid_body_map_iterator;
typedef std::map<int, btCollisionObject*>::iterator collision_object_map_iterator;
*/


class BulletWrapper
{
public:
    static BulletWrapper& SharedInstance();

    // World stuff
    static int CreateDynamicsWorld(lua_State* L);
    static void SetWorldGravity(lua_State* L);
    static void StepSimulation(lua_State* L);

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

private:
    BulletWrapper();
    /*
public:
    static BulletWrapper& shared_instance();

    // World methods
    static void create_dynamics_world();
    static void reset_dynamics_world();
    static void step_simulation();

    // Rigid body methods
    static int create_rigid_body(int collision_object_handle);
    static double* get_transform(int handle) const;
    static double* get_position(int handle) const;
    static double get_oriantation(int handle) const;
    static void set_transform(int handle, double* transform);
    static void set_position(int handle, double* position);
    static void set_orientation(int handle, double* orientation);
    static bool delete_rigid_body(int handle);

    // Tunnel methods
    static void create_tunnel(std::vector<btVector3> path, double radius, double circle);
    static void reset_tunnel(std::vector<btVector3> path, double radius, double circle);

    // Collision object methods
    static int create_sphere_shape(double radius);
    static int create_box_shape(double x, double y, double z);
    static int create_cylinder_shape(double radius, double height);
    static int create_triangle_mesh_shape(std::string filename);
    static void set_shape_scaling(int handle, double x_scale, double y_scale, double z_scale);
    static bool delete_shape(int handle);


private:
    BulletWrapper();

    // World
    static btDynamicsWorld* dynamics_world;
    static btDefaultCollisionConfiguration* collision_configuration;
    static btCollisionDispatcher* dispatcher;
    static btBroadphaseInterface* broadphase;
    static btSequentialImpulseConstraintSolver* solver;

    // Standard rigid body and collision object handles
    static rigid_body_map rigid_bodies;  int rigid_bodies_counter;
    static collision_object_map collision_objects; int collision_objects_counter;
    static std::map<btRigidBody*, btCollisionObject*> rigid_bodies_to_collision_objects;

    // Tunnel primitives
    static btCompoundShape tunnel;
    static btBoxShape* tunnel_primitives;
    */
};


#endif // BULLETWRAPPER_H
