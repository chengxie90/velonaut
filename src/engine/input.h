#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <map>
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
    
    std::map<uint, std::string> dictionary_;
    
private:
    friend class App;
    SINGLETON(Input)

    void fillDictionary();
    
    std::vector<InputListener*> listeners_;
    
};

#endif // INPUT_H
