#ifndef OGRETEST_H
#define OGRETEST_H

#include <stdio.h>
#include <OGRE/Ogre.h>

using namespace Ogre;

class OgreTest : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    OgreTest();
    void init();
    void run();
    void render();

private:
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;

    virtual void windowResized (RenderWindow *rw);
    virtual bool windowClosing (RenderWindow *rw);
    virtual void windowClosed(RenderWindow *rw);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted( const Ogre::FrameEvent& evt);
};

#endif // OGRETEST_H
