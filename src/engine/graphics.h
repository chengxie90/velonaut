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
    
    static Graphics* GetInstance();
private:
    SINGLETON(Graphics)
    friend class App;
    
    void initWindow();
    void initResources();
    
    // tmp
    void createScene();
    
    SDL_Window* window_ = NULL;
    Ogre::Root* root_ = NULL;
    Ogre::RenderWindow* renderWindow_ = NULL;
    Ogre::SceneManager* scene_ = NULL;
    
private:
    static int LSceneCreate(lua_State* state);
    static int LSceneSetActive(lua_State* state);
    
    static int LNodeCreate(lua_State* state);
    static int LNodeSetPosition(lua_State* state);
    static int LNodeLookAt(lua_State* state);
    static int LNodeAttachObject(lua_State* state);
    
    static int LCameraCreate(lua_State* state);
    static int LCameraSetNear(lua_State* state);
    static int LCameraSetFar(lua_State* state);
    
    static int LLightCreate(lua_State* state);
    static int LLightSetDiffuse(lua_State* state);
    
    static int LEntityCreate(lua_State* state);
};

#endif // GRAPHICS_H
