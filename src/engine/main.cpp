// #include "ogretest.h"
#ifdef __APPLE__
#include "macosx.h"
#endif

#include "app.h"

#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>
#include <Ogre.h>

using namespace std; 
using namespace Ogre;

int main(int argc, char *argv[]) {
    assert(!SDL_Init(SDL_INIT_EVERYTHING));
    
    SDL_Window* win = SDL_CreateWindow("Velonaut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       800, 600,
                                       SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    assert(win);
    
#ifndef __APPLE__
    SDL_GLContext glcontext = NULL;
    glcontext = SDL_GL_CreateContext(win);
    assert(glcontext);
#endif
        
    Ogre::Root * root = new Ogre::Root();
    
#ifdef __APPLE__
    root->loadPlugin("RenderSystem_GL");
#elif __LINUX__
    root->loadPlugin("./Linux/Debug/RenderSystem_GL.so");   
#endif
    
    root->setRenderSystem( root->getAvailableRenderers()[0] );
    root->initialise( false );
    
    Ogre::NameValuePairList params;
    
#ifdef __APPLE__
    params["externalGLControl"] = "1";
    params["externalWindowHandle"] = osx_cocoa_view(win);
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
#elif __LINUX__
    params["externalGLControl"] = "1";
    params["currentGLContext"] = "1";
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    assert(SDL_GetWindowWMInfo(win, &info));
    params["parentWindowHandle"] = Ogre::StringConverter::toString(info.info.x11.window);
#endif
    
#ifndef __APPLE__
    SDL_GL_MakeCurrent(win, glcontext);
    SDL_GL_SetSwapInterval(1);
#endif
    
    Ogre::RenderWindow * renderwindow = root->createRenderWindow("Legion::core::ogre", 800, 600, false, &params);
    
    SDL_ShowWindow(win);
    renderwindow->setVisible(true);
    
    SceneManager* scene = root->createSceneManager(Ogre::ST_GENERIC);

    Camera* camera = scene->createCamera("PlayerCam");
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1000);

    Ogre::Viewport* vp = renderwindow->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(1,0,0));

    // Alter the camera aspect ratio to match the viewport
    camera->setAspectRatio(800.f / 600.f);
    
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                cout << e.key.keysym.sym << endl;
                vp->setBackgroundColour(Ogre::ColourValue(0,1,0));
            }
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        //root->_fireFrameStarted();
        root->renderOneFrame();
        
#ifndef __APPLE__
        SDL_GL_SwapWindow(win);
#endif
    }

    SDL_DestroyWindow(win);    
    SDL_Quit();
    
    return 0;
}

