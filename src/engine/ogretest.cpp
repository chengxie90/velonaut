#include "ogretest.h"

OgreTest::OgreTest():mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0)
{
    mRoot = new Ogre::Root("", "", "");
    mRoot->loadPlugin("./RenderSystem_GL_d.so");

    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        //return true;
    }
    else
    {
       // return false;
    }

    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

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

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


    // create scene with meshes here

    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    std::string str = windowHndStr.str();
    pl.insert(std::make_pair(std::string("WINDOW"), str));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    windowResized(mWindow);
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
    mRoot->startRendering();
}

bool OgreTest::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    std::cout << "frame rendering" << std::endl;
    return true;
}

bool OgreTest::keyPressed( const OIS::KeyEvent &arg )
{
    std::cout << "mouse pressed!" << std::endl;
    return true;
}

bool OgreTest::keyReleased( const OIS::KeyEvent &arg )
{
    std::cout << "mouse released!" << std::endl;
    return true;
}

bool OgreTest::mouseMoved( const OIS::MouseEvent &arg )
{
    std::cout << "mouse moved!" << std::endl;
    return true;
}

bool OgreTest::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

bool OgreTest::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

/*
void OgreTest::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    /*
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;

}

*/
