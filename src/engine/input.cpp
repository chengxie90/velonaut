#include "input.h"
#include <SDL2/SDL.h>
#include "app.h"

Input::Input()
{
}

void Input::init()
{
    
}

void Input::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                App::GetApp()->terminate();
            }
        }
        if (e.type == SDL_QUIT) {
            App::GetApp()->terminate();
        }
    }
}

void Input::shutdown()
{
    
}

Input *Input::GetInstance()
{
    return App::GetApp()->GetInput();
}
