#ifndef GRAPHICSTEST_H
#define GRAPHICSTEST_H

#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>
#include "common.h"

class GraphicsTest
{
public:
    void init();
    void render();
    void shutdown();

    static GraphicsTest* GetInstance();
private:
    SINGLETON(GraphicsTest)
    friend class GameTest;

    void initWindow();
    void initResources();

    // tmp
    void createScene();

    SDL_Window* window_ = NULL;
    Ogre::Root* root_ = NULL;
    Ogre::RenderWindow* renderWindow_ = NULL;
    Ogre::SceneManager* scene_ = NULL;
};


#endif // GRAPHICSTEST_H
