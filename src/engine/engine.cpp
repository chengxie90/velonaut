#include <OGRE/Ogre.h>

using namespace Ogre;

void bullet_test()
{
    String config_file = "";
    String plugin_file = "";
    String log_file = "ogre.log";
    
    Root* root = new Root(config_file, plugin_file, log_file);
    
    root->loadPlugin("RenderSystem_GL");
    
    delete root;
}

