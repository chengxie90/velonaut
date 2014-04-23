#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <SDL2/SDL.h>

class InputListener
{
public:

    virtual void onMouseDown( SDL_Event e ) = 0;
    virtual void onMouseUp( SDL_Event e) = 0;
    virtual void onMouseMove( SDL_Event e ) = 0;

    virtual void onKeyDown( SDL_Event e ) = 0;
    virtual void onKeyUp( SDL_Event e ) = 0;

protected:

    Uint8* keys_;
};

#endif // INPUTLISTENER_H
