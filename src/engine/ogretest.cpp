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
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setPosition(Ogre::Vector3(-75, 150, 250));
        pointLight->setDiffuseColour(1.0, 0.0, 0.0);
        pointLight->setSpecularColour(1.0, 0.0, 0.0);

    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

//    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

//    // create scene with meshes here
    
//    assert(!SDL_Init(SDL_INIT_EVERYTHING));
    
//    OIS::ParamList pl;
//    size_t windowHnd = 0;
//    //std::ostringstream windowHndStr;
//    mWindow->getCustomAttribute("WINDOW", &windowHnd);
//    assert(windowHnd);
    
    //SDL_Window* win = SDL_CreateWindowFrom((void *)windowHnd);
    //assert(win);
    
//    windowHndStr << windowHnd;
//    std::string str = windowHndStr.str();
//    pl.insert(std::make_pair(std::string("WINDOW"), str));

//    mInputManager = OIS::InputManager::createInputSystem( pl );

//    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
//    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

//    mMouse->setEventCallback(this);
//    mKeyboard->setEventCallback(this);

    windowResized(mWindow);
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);

//    Ogre::String lNameOfResourceGroup = "Mission 1 : Deliver Tom";
//    {
//        Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
//        lRgMgr.createResourceGroup(lNameOfResourceGroup);

//        Ogre::String lDirectoryToLoad = "data/meshes";
//        bool lIsRecursive = false;
//        lRgMgr.addResourceLocation(lDirectoryToLoad, "FileSystem", lNameOfResourceGroup, lIsRecursive);

//        // The function 'initialiseResourceGroup' parses scripts if any in the locations.
//        lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);

//        // Files that can be loaded are loaded.
//        lRgMgr.loadResourceGroup(lNameOfResourceGroup);

//        // Now the loaded Mesh is available from its ResourceGroup,
//        // as well as from the Ogre::MeshManager. A shared pointer to
//        // it can be accessed by : Ogre::MeshManager::getSingleton().getByName(name_of_the_mesh);

//        // Now I can create Entities using that mesh.
//        Ogre::String lNameOfTheMesh = "ninja.mesh";
//        int lNumberOfEntities = 2;
//        for(int iter = 0; iter < lNumberOfEntities; ++iter)
//        {
//            Ogre::Entity* lEntity = mSceneMgr->createEntity(lNameOfTheMesh);
//            // Now I attach it to a scenenode, so that it becomes present in the scene.
//            Ogre::SceneNode* lNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//            lNode->attachObject(lEntity);
//            // I move the SceneNode so that it is visible to the camera.
//            float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
//            lNode->scale(0.2,0.2,0.2);
//            lPositionOffset = lPositionOffset * 20;
//            lNode->translate(lPositionOffset, lPositionOffset, -200.0f);
//            // The loaded mesh will be white. This is normal.
//        }
//    }
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

