#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>

class App {
public:
    bool init(int argc, char *argv[]);
    void run();
    void shutdown();
    
private:
    void initWindow();
    void initLua();
    void initScene();
    
    SDL_Window* window_ = NULL;
    
    Ogre::Root* root_ = NULL;
    Ogre::RenderWindow* renderWindow_  = NULL;
    Ogre::SceneManager* scene_ = NULL;
};

#endif
