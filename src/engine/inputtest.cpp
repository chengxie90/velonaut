#include "inputtest.h"
#include <SDL2/SDL.h>
#include "gametest.h"

InputTest::InputTest()
{
}

void InputTest::init()
{

}

void InputTest::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                GameTest::GetApp()->terminate();
            }
        }
        if (e.type == SDL_QUIT) {
            GameTest::GetApp()->terminate();
        }
    }
}

void InputTest::shutdown()
{

}

InputTest *InputTest::GetInstance()
{
    return GameTest::GetApp()->GetInput();
}