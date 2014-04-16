#include "graphicstest.h"
#include <OGRE/Ogre.h>
#include <SDL2/SDL_syswm.h>
#include "common.h"
#ifdef __APPLE__
#include "macosx.h"
#endif
#include "gametest.h"

using namespace Ogre;
using namespace std;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static string WINDOW_TITLE = "Velonaut";

GraphicsTest::GraphicsTest() {}

void GraphicsTest::init()
{
    assert(!SDL_Init(SDL_INIT_EVERYTHING));

    root_ = new Root();
    assert(root_);

#ifdef __APPLE__
    root_->loadPlugin("RenderSystem_GL");
#elif __LINUX__
    // TODO: fix Linux path problem
    root_->loadPlugin("./Linux/Debug/RenderSystem_GL.so");
#endif

    RenderSystem* renderer = root_->getAvailableRenderers()[0];
    assert(renderer);
    root_->setRenderSystem(renderer);

    root_->initialise(false);

    initWindow();
    initResources();

    createScene();
}

void GraphicsTest::render()
{
    root_->_fireFrameStarted();
    root_->renderOneFrame();
    root_->_fireFrameRenderingQueued();
    root_->_fireFrameEnded();

#ifndef __APPLE__
    SDL_GL_SwapWindow(window_);
#endif
}

void GraphicsTest::shutdown()
{
    delete root_;

    SDL_DestroyWindow(window_);
    SDL_Quit();
}

GraphicsTest *GraphicsTest::GetInstance()
{
    return GameTest::GetApp()->GetGraphics();
}

void GraphicsTest::initWindow()
{
    window_ = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_HIDDEN
                               | SDL_WINDOW_OPENGL
                               | SDL_WINDOW_RESIZABLE);
    assert(window_);

    // Apple's GL context is managed by Ogre
#ifndef __APPLE__
    SDL_GLContext glcontext = NULL;
    glcontext = SDL_GL_CreateContext(window_);
    assert(glcontext);
#endif

    NameValuePairList params;
#ifdef __APPLE__
    params["externalGLControl"] = "1";
    params["externalWindowHandle"] = osx_cocoa_view(window_);
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
#elif __LINUX__
    params["externalGLControl"] = "1";
    params["currentGLContext"] = "1";
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    assert(SDL_GetWindowWMInfo(window_, &info));
    params["parentWindowHandle"] = StringConverter::toString(info.info.x11.window);
#endif

#ifndef __APPLE__
    SDL_GL_MakeCurrent(window_, glcontext);
    SDL_GL_SetSwapInterval(1);
#endif

    renderWindow_ = root_->createRenderWindow(WINDOW_TITLE,
                                              WINDOW_WIDTH,
                                              WINDOW_HEIGHT,
                                              false,
                                              &params);

    // Both calls are needed
    SDL_ShowWindow(window_);
    renderWindow_->setVisible(true);
}

void GraphicsTest::initResources()
{
    // For now just using the default resource group for simplicity
    ResourceGroupManager& resGroupManager = ResourceGroupManager::getSingleton();
    resGroupManager.addResourceLocation("data/meshes", "FileSystem");
    resGroupManager.addResourceLocation("data/materials", "FileSystem");
    resGroupManager.initialiseAllResourceGroups();
    resGroupManager.loadResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void GraphicsTest::createScene()
{
    scene_ = root_->createSceneManager(Ogre::ST_GENERIC);
    Camera* camera = scene_->createCamera("cam");

    Viewport* vp = renderWindow_->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0.0, 0.0, 0.1));

    camera->setAspectRatio((float)WINDOW_WIDTH / WINDOW_HEIGHT);

    /*
    //Entity* entity = scene_->createEntity("sphere",Ogre::SceneManager::PT_SPHERE);
    Entity* entity = scene_->createEntity("ninja.mesh");
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    entity->setMaterial(material);
    SceneNode* node = scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);

    Entity* entity2 = scene_->createEntity("sphere",Ogre::SceneManager::PT_SPHERE);
    entity2->setMaterialName("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    SceneNode* node2 = scene_->getRootSceneNode()->createChildSceneNode();
    node2->setPosition(0,0,0);
    node2->attachObject(entity2);
    */

    scene_->setAmbientLight(ColourValue(0.3, 0.3, 0.3));

    SceneNode* camNode = scene_->getRootSceneNode()->createChildSceneNode();

    camNode->setPosition(Vector3(500, 100, 0));
    camNode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);

    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1000);

    camNode->attachObject(camera);

    Ogre::Light* pointLight = scene_->createLight();
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(camNode->getPosition());
    pointLight->setDiffuseColour(0.7, 0.7, 1.0);
}

Ogre::SceneNode* GraphicsTest::createSphere(std::vector<double> position, std::vector<double> orientation, double radius)
{
    Entity* entity = GraphicsTest::GetInstance()->scene_->createEntity(Ogre::SceneManager::PT_SPHERE);
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    entity->setMaterial(material);
    SceneNode* node = GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(position[0], position[1], position[2]);
    node->setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
    node->scale(radius, radius, radius);
    return node;
}

Ogre::SceneNode* GraphicsTest::createPlane(std::vector<double> position, std::vector<double> orientation)
{
    Plane plane(Ogre::Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                           1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
    Entity* entity = GraphicsTest::GetInstance()->scene_->createEntity("plane");
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    entity->setMaterial(material);
    SceneNode* node = GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(position[0], position[1], position[2]);
    node->setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
    return node;
}

std::vector<double> GraphicsTest::getSceneNodePosition(Ogre::SceneNode* node)
{
    Vector3 pos = node->getPosition();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = pos.x;
    ret[1] = pos.y;
    ret[2] = pos.z;
    return ret;
}

void GraphicsTest::setSceneNodePosition(Ogre::SceneNode* node, std::vector<double> pos)
{
    node->setPosition(Vector3(pos[0], pos[1], pos[2]));
}

std::vector<double> GraphicsTest::getSceneNodeOrientation(Ogre::SceneNode* node)
{
    Quaternion ori = node->getOrientation();
    std::vector<double> ret = std::vector<double>(4);
    ret[0] = ori.w;
    ret[1] = ori.x;
    ret[2] = ori.y;
    ret[3] = ori.z;
    return ret;
}

void GraphicsTest::setSceneNodeOrientation(Ogre::SceneNode* node, std::vector<double> ori)
{
    node->setOrientation(Quaternion(ori[0], ori[1], ori[2], ori[3]));
}
