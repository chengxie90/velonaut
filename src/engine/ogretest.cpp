#include "ogretest.h"
#include <SDL2/SDL.h>

using namespace std;

OgreTest::OgreTest():mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0)
{

}

void OgreTest::init() {
    mRoot = new Ogre::Root("", "", "");

#ifdef __APPLE__
    mRoot->loadPlugin("RenderSystem_GL");
#else
    mRoot->loadPlugin("./Linux/Debug/RenderSystem_GL_d.so");
#endif
    
    mRoot->setRenderSystem(mRoot->getAvailableRenderers()[0]);
    
    mRoot->initialise(false);
    
    NameValuePairList params;
    params["macAPI"] = "cocoa";       
    params["macAPICocoaUseNSView"] = "true";
    
    mWindow = mRoot->createRenderWindow("Velonaut", 800, 600, false, &params);

    // Get the SceneManager, in this case a generic one


    windowResized(mWindow);
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
}

void OgreTest::run()
{
    mRoot->startRendering();
}

void OgreTest::render()
{
    mRoot->renderOneFrame();
}

bool OgreTest::frameStarted(const Ogre::FrameEvent& evt)
{
//    mMouse->capture();
//    mKeyboard->capture();

//    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
//         return false;
    
//    SDL_Event e;
//    while (SDL_PollEvent(&e)) {
//        if (e.type == SDL_KEYDOWN) {
//            cout << 123 << endl;
//        }
//    }

    return true;
}

bool OgreTest::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return true;
}

void OgreTest::windowResized( RenderWindow* rw ) {

//    unsigned int width, height, depth;
//    int left, top;
//    rw->getMetrics(width, height, depth, left, top);

//    const OIS::MouseState &ms = mMouse->getMouseState();
//    ms.width = width;
//    ms.height = height;
}

void OgreTest::windowClosed( RenderWindow* rw ) {
//    std::cout << "CLOSED" << std::endl;
//    if(rw == mWindow) {
//        if(mInputManager)
//        {
//            mInputManager->destroyInputObject( mMouse );
//            mInputManager->destroyInputObject( mKeyboard );
            
//            OIS::InputManager::destroyInputSystem(mInputManager);
//            mInputManager = 0;
//        }
//    }

//    mRoot->shutdown();
//    exit(0);
}


bool OgreTest::windowClosing( RenderWindow* rw ) {
//    std::cout << "CLOOOOOSE" << std::endl;
//    mRoot->removeFrameListener(this);
    return true;
}

