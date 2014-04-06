#include <OGRE/Ogre.h>
#include <unistd.h>

using namespace Ogre;
using namespace std;

void bullet_test()
{
    String config_file = "";
    String plugin_file = "";
    String log_file = "ogre.log";
    
    Root* root = new Root(config_file, plugin_file, log_file);
    assert(root);
    
    root->loadPlugin("RenderSystem_GL");
    
    RenderSystem* render_sys = root->getAvailableRenderers()[0];
    
    root->setRenderSystem(render_sys);
    
    root->initialise(false, "", "");
    
    RenderWindow* window = NULL;
    
    NameValuePairList params;
    params["FSAA"] = "0";
    params["vsync"] = "true";
    params["macAPI"] = "cocoa";
    
    window = root->createRenderWindow("Velonaut", 800, 600, false, &params);
    
    while (!window->isClosed()) {
        WindowEventUtilities::messagePump();
        root->renderOneFrame();
    }
    
    delete root;
}

