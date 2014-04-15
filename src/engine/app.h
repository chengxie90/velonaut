#ifndef APP_H
#define APP_H

#include "common.h"

class Graphics;
class Input;
class LuaManager;

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
    
private:
    bool terminated_ = false;
    
    Graphics* graphics_ = NULL;
    Input* input_ = NULL;
    LuaManager* luaManager_ = NULL;
};

#endif
