#include <SDL2/SDL.h>
#include "input.h"
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

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            for (int i = 0; i < listeners_.size(); ++i) {
                listeners_[i]->onMouseDown(e);
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            for (int i = 0; i < listeners_.size(); ++i) {
                listeners_[i]->onMouseUp(e);
            }
        }

        if (e.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < listeners_.size(); ++i) {
                listeners_[i]->onMouseMove(e);

            }
        }
    }
}

void Input::shutdown()
{
    
}

void Input::addListener(InputListener *listener) {
    listeners_.push_back(listener);
}

void Input::callListeners() {

}

Input *Input::GetInstance()
{
    return App::GetApp()->GetInput();
}
