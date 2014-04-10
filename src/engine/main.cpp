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

Ogre::String OSX_cocoa_view( SDL_Window * window ) {
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    assert(SDL_GetWindowWMInfo(window, &info));
    NSWindow* nswindow = info.info.cocoa.window;
    assert(nswindow);
    NSView *view = [nswindow contentView];
    assert(view);
    return Ogre::StringConverter::toString( (unsigned long)view );
}

int main(int argc, char *argv[]) {
    assert(!SDL_Init(SDL_INIT_EVERYTHING));
    
    SDL_Window* win = SDL_CreateWindow("Velonaut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       800, 600,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    assert(win);
        
    Ogre::Root * root = new Ogre::Root();
    root->loadPlugin( "RenderSystem_GL" );
    
    root->setRenderSystem( root->getAvailableRenderers()[0] );
    root->initialise( false );
    
    Ogre::NameValuePairList params;
    
    params["externalGLControl"] = "1";
    params["externalWindowHandle"] = OSX_cocoa_view(win);
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
    
    Ogre::RenderWindow * renderwindow = root->createRenderWindow("Legion::core::ogre", 800, 600, false, &params);
    
    renderwindow->setVisible( true );
    
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
        root->_fireFrameStarted();
        root->renderOneFrame();
    }

    SDL_DestroyWindow(win);    
    SDL_Quit();
    
    return 0;
}

