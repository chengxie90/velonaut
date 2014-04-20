#ifndef APP_H
#define APP_H

#include "common.h"
#include <OGRE/Ogre.h>

class Graphics;
class Input;
class LuaManager;
class Network;
class Ui;
class Physics;

class App {
public:
    bool init(int argc, char *argv[]);
    void run();
    void shutdown();
    void terminate();

    static App* GetApp();
    static Graphics* GetGraphics();
    static Input* GetInput();
    static LuaManager* GetLuaManager();
    static Network* GetNetwork();
    static Ui* GetUi();
    static Physics* GetPhysics();
    
private:
    bool terminated_ = false;
    
    Graphics* graphics_ = NULL;
    Input* input_ = NULL;
    LuaManager* luaManager_ = NULL;
    Network* network_ = NULL;
    Ui* ui_ = NULL;
    Physics* physics_ = NULL;
};

#endif
