#ifndef GRAPHICSTEST_H
#define GRAPHICSTEST_H

#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>
#include "common.h"

#include <vector>

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

    static Ogre::SceneNode* createSphere(std::vector<double> position, std::vector<double> orientation, double radius);
    static Ogre::SceneNode* createPlane(std::vector<double> position, std::vector<double> orientation);

    static std::vector<double> getSceneNodePosition(Ogre::SceneNode* node);
    static void setSceneNodePosition(Ogre::SceneNode* node, std::vector<double>pos);
    static std::vector<double> getSceneNodeOrientation(Ogre::SceneNode* node);
    static void setSceneNodeOrientation(Ogre::SceneNode* node, std::vector<double> ori);

};


#endif // GRAPHICSTEST_H
