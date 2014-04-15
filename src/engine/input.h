#ifndef INPUT_H
#define INPUT_H

#include "common.h"

class Input
{
public:
    void init();
    void update();
    void shutdown();
    
    static Input* GetInstance();
    
private:
    friend class App;
    SINGLETON(Input)
};

#endif // INPUT_H
