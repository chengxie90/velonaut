#include <OGRE/Ogre.h>

using namespace Ogre;

class Listener : public FrameListener {
    virtual bool frameStarted(const FrameEvent& evt)
    {
        printf("!23\n");
        return true;
    }
};

int main() {
    Root *root = new Root();
    root->loadPlugin("RenderSystem_GL");

    RenderWindow* window = NULL;
    if (root->showConfigDialog()) {
        window = root->initialise(true, "title");
    }
    else {
        assert(false);
    }

    SceneManager* scene = root->createSceneManager(Ogre::ST_GENERIC);

    Camera* cam = scene->createCamera("cam");
    cam->setNearClipDistance(1);
    cam->setFarClipDistance(1000);
    cam->setAspectRatio(1);

    Viewport* vp = window->addViewport(cam);
    vp->setBackgroundColour(ColourValue(1,0,0));

    Listener listener;

    root->addFrameListener(&listener);

    root->startRendering();

    delete root;
    return 0;
}