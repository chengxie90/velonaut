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
    
    // tmp
    void createScene();
    
    SDL_Window* window_ = NULL;

    Ogre::Root* root_ = NULL;
    Ogre::Camera* defaultCamera_ = NULL;
    Ogre::Viewport* viewport_ = NULL;
    


    // lua bindings
private:

    
    struct Scene {
        static int lcreateNode(lua_State *);
        // TODO: destroy
        
        static int lcreateCamera(lua_State *);
        // TODO: destroy
        
        static int lsetMainCamera(lua_State *);
        
        static int lcreateLight(lua_State *);
        // TODO: destroy
        
        static int lsetAmbientLight(lua_State *);
        
        static int lcreateEntity(lua_State *);
        // TODO: destroy
    };
    
    struct Node {
        static int lsetPosition(lua_State *);
        static int lposition(lua_State *);
        static int llookAt(lua_State *); 
        static int lattachObject(lua_State *);
    };
    
    struct Camera {
        static int lsetNear(lua_State *); 
        static int lsetFar(lua_State *);
        static int lsetFOV(lua_State *);
    };
    
    struct Light {
        static int lsetType(lua_State *);
        static int lsetDiffuse(lua_State *); 
    };
    
    struct Entity {
        
    };
};

#endif // GRAPHICS_H
