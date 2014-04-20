#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include "common.h"
#include "inputlistener.h"

class Input
{
public:
    void init();
    void update();
    void shutdown();
    void addListener( InputListener* listener );
    
    static Input* GetInstance();
    
private:
    friend class App;
    SINGLETON(Input)
    void callListeners();

    std::vector<InputListener*> listeners_;
};

#endif // INPUT_H
