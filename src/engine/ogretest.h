#ifndef OGRETEST_H
#define OGRETEST_H


//#include <bullet/btBulletDynamicsCommon.h>
#include <stdio.h>
#include <OGRE/Ogre.h>
#include <OIS.h>

//#include <lua/lua.hpp>
//#include <RakNet/RakPeerInterface.h>


using namespace Ogre;

class OgreTest : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
    OgreTest();
    void init();

private:
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;

    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    virtual void windowResized (RenderWindow *rw);
    virtual bool windowClosing (RenderWindow *rw);
    virtual void windowClosed(RenderWindow *rw);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted( const Ogre::FrameEvent& evt);

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

};

#endif // OGRETEST_H
