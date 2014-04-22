#ifndef INPUTTEST_H
#define INPUTTEST_H

#include "common.h"

class InputTest
{
public:
    void init();
    void update();
    void shutdown();

    static InputTest* GetInstance();

private:
    friend class GameTest;
    SINGLETON(InputTest)
};

#endif // INPUTTEST_H
