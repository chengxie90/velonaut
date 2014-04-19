#ifndef GRAPHICSTEST_H
#define GRAPHICSTEST_H

#include <SDL2/SDL.h>
#include <OGRE/Ogre.h>
#include "beziertest.h"
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
    friend class InputTest;

    void initWindow();
    void initResources();

    // tmp
    void createScene();

    SDL_Window* window_ = NULL;
    Ogre::Root* root_ = NULL;
    Ogre::RenderWindow* renderWindow_ = NULL;
    Ogre::SceneManager* scene_ = NULL;
    Ogre::SceneNode* camNode_ = NULL;

    static Ogre::SceneNode* createEllipsoid(std::vector<double> position, std::vector<double> orientation, std::vector<double> scale);
    static Ogre::SceneNode* createBox(std::vector<double> position, std::vector<double> orientation, std::vector<double> scale);
    static Ogre::SceneNode* createPlane(std::vector<double> position, std::vector<double> orientation);

    static std::vector<double> getSceneNodePosition(Ogre::SceneNode* node);
    static void setSceneNodePosition(Ogre::SceneNode* node, std::vector<double>pos);
    static std::vector<double> getSceneNodeOrientation(Ogre::SceneNode* node);
    static void setSceneNodeOrientation(Ogre::SceneNode* node, std::vector<double> ori);

    static std::vector<double> getCameraPosition();
    static void setCameraPostion(std::vector<double> position);
    static void setCameraOrientation(std::vector<double> orientation);
    static std::vector<double> getCameraDerivedDirection();
    static std::vector<double> getCameraDerivedUp();

    static void createTunnel(unsigned int seed);
    static Ogre::Vector3 getRandomPoint(double min, double max);
};


#endif // GRAPHICSTEST_H
