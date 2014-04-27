#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>
#include <lua/lua.hpp>
#include "common.h"

class Graphics {
public:
    void init();
    void initLua();
    void render();
    void shutdown();
    Ogre::RenderWindow* renderWindow_ = NULL;
    Ogre::SceneManager* scene_ = NULL;
    
    static Graphics* GetInstance();

    static int lcreateScene(lua_State *);
    // TODO: destroy

    static int lsetActiveScene(lua_State *);
    static int lsetBackgroundColor(lua_State *);
    
private:
    SINGLETON(Graphics)
    friend class App;
    
    void initWindow();
    void initResources();
    
    SDL_Window* window_ = NULL;

    Ogre::Root* root_ = NULL;
    Ogre::Camera* defaultCamera_ = NULL;
    Ogre::Viewport* viewport_ = NULL;
    
private:

    // we need to keep shared pointers to keep them from being deleted
    std::map<Ogre::Mesh*, Ogre::MeshPtr> meshMap_;
    std::map<Ogre::Material*, Ogre::MaterialPtr> materialMap_;
    
    struct Scene {
        static int lcreate(lua_State *);
        static int lsetMainCamera(lua_State *);
        static int lsetAmbientLight(lua_State *);
    };
    
    struct Node {
        static int lcreate(lua_State *);
        static int lsetParent(lua_State *);
        
        static int lposition(lua_State *);
        static int lsetPosition(lua_State *);
        static int lorientation(lua_State *);
        static int lsetOrientation(lua_State *);
        static int llookAt(lua_State *); 
        
        static int lattachObject(lua_State *);
        static int ldetachObject(lua_State *);
        
        static int lsetScale(lua_State *);
        static int llocalX(lua_State *);
        static int llocalY(lua_State *);
        static int llocalZ(lua_State *);
    };
    
    struct Camera {
        static int lcreate(lua_State *);
        static int lsetNear(lua_State *); 
        static int lsetFar(lua_State *);
        static int lsetFOV(lua_State *);
    };
    
    struct Light {
        static int lcreate(lua_State *);
        static int lsetType(lua_State *);
        static int lsetDiffuse(lua_State *); 
    };
    
    struct Entity {
        static int lcreateMesh(lua_State *);
        static int lcreateSphere(lua_State *);
        static int lcreateBox(lua_State *);
        static int lsetMaterial(lua_State *);
    };
    
    struct Mesh {
        static int lcreate(lua_State *);
    };
    
    struct MeshBuilder {
        static int lcreate(lua_State *);
        static int lposition(lua_State *);
        static int lnormal(lua_State *);
        static int lindex(lua_State *);
        static int lgetMesh(lua_State *);
    };
    
    struct Material {
        static int lcreate(lua_State *);
    };
    
    struct Particle {
        static int lcreate(lua_State *);
    };

    struct VQHelper {
        static int lgetQuaternionFromAxes(lua_State *);
        static int lgetQuaternionFromAngleAxis(lua_State *);
        static int langleBetween(lua_State *);
        static int lrotationTo(lua_State *);
        static int lapplyRotationTo(lua_State *);
    };
};

#endif // GRAPHICS_H
