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

    Entity* entity = scene_->createEntity("ninja.mesh");
    SceneNode* node = scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);

    scene_->setAmbientLight(ColourValue(0.3, 0.3, 0.3));

    SceneNode* camNode = scene_->getRootSceneNode()->createChildSceneNode();

    camNode->setPosition(Vector3(500, 0, 0));
    camNode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);

    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1000);

    camNode->attachObject(camera);

    Ogre::Light* pointLight = scene_->createLight();
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(camera->getPosition());
    pointLight->setDiffuseColour(0.7, 0.7, 1.0);
}
