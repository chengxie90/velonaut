#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <SDL2/SDL.h>

class InputListener
{
public:

    virtual void onMouseDown( SDL_Event e ) = 0;
    virtual void onMouseUp( SDL_Event e) = 0;
    virtual void onMouseMove( SDL_Event e ) = 0;
};

#endif // INPUTLISTENER_H
