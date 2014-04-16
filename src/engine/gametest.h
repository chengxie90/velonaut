#ifndef GAMETEST_H
#define GAMETEST_H

#include "common.h"

class Graphics;
class Input;
class Physics;

class GameTest {
public:
    bool init(int argc, char *argv[]);
    void run();
    void shutdown();
    void terminate();

    static GameTest* GetApp();
    static Graphics* GetGraphics();
    static Input* GetInput();
    static Physics* GetPhysics();

private:
    bool terminated_ = false;

    Graphics* graphics_ = NULL;
    Input* input_ = NULL;
    Physics* physics_ = NULL;
};

#endif // GAMETEST_H
