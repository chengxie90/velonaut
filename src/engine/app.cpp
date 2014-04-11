#include "app.h"
#ifdef __APPLE__
#include "macosx.h"
#endif

#include <SDL2/SDL_syswm.h>
#include "common.h"

using namespace Ogre;
using namespace std;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static string WINDOW_TITLE = "Velonaut";

bool App::init(int argc, char *argv[])
{
    initWindow();
    initLua();
    initScene();
    return true;
}

void App::run()
{
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        root_->_fireFrameStarted();
        root_->renderOneFrame();
        root_->_fireFrameRenderingQueued();
        root_->_fireFrameEnded();
        
#ifndef __APPLE__
        SDL_GL_SwapWindow(window_);
#endif
    }
}

void App::shutdown()
{
    delete root_;
    
    SDL_DestroyWindow(window_);    
    SDL_Quit();
}

void App::initWindow()
{
    assert(!SDL_Init(SDL_INIT_EVERYTHING));
    
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

void App::initLua()
{
    
}

void App::initScene()
{
    // TODO: init from Lua
    scene_ = root_->createSceneManager(Ogre::ST_GENERIC);
    Camera* camera = scene_->createCamera("cam");
    Viewport* vp = renderWindow_->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));
    camera->setAspectRatio((float)WINDOW_WIDTH / WINDOW_HEIGHT);
}
