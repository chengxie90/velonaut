#include <SDL2/SDL.h>
#include <string>
#include "input.h"
#include "app.h"

Input::Input()
{
}

void Input::init()
{
    fillDictionary();
}

void Input::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                App::GetApp()->terminate();
            }
            for (int i = 0; i < listeners_.size(); ++i) {
                listeners_[i]->onKeyDown(e);
            }
        }
        if (e.type == SDL_KEYUP) {
            for (int i = 0; i < listeners_.size(); ++i) {
                listeners_[i]->onKeyUp(e);
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

Input *Input::GetInstance()
{
    return App::GetApp()->GetInput();
}

void Input::fillDictionary()
{
    dictionary_[SDL_BUTTON_LEFT]    = "mouse_left";
    dictionary_[SDL_BUTTON_RIGHT]   = "mouse_right";
    dictionary_[SDLK_UP]            = "key_up";
    dictionary_[SDLK_DOWN]          = "key_down";
    dictionary_[SDLK_LEFT]          = "key_left";
    dictionary_[SDLK_RIGHT]         = "key_right";
    dictionary_[SDLK_SPACE]         = "key_space";
    dictionary_[SDLK_LSHIFT]        = "key_lshift";
    dictionary_[SDLK_RSHIFT]        = "key_rshift";
    dictionary_[SDLK_LCTRL]         = "key_lctrl";
    dictionary_[SDLK_RCTRL]         = "key_rctrl";
    dictionary_[SDLK_RETURN]        = "key_return";
    dictionary_[SDLK_ESCAPE]        = "key_esc";
    dictionary_[SDLK_0]             = "key_0";
    dictionary_[SDLK_1]             = "key_1";
    dictionary_[SDLK_2]             = "key_2";
    dictionary_[SDLK_3]             = "key_3";
    dictionary_[SDLK_4]             = "key_4";
    dictionary_[SDLK_5]             = "key_5";
    dictionary_[SDLK_6]             = "key_6";
    dictionary_[SDLK_7]             = "key_7";
    dictionary_[SDLK_8]             = "key_8";
    dictionary_[SDLK_9]             = "key_9";
    dictionary_[SDLK_a]             = "key_a";
    dictionary_[SDLK_b]             = "key_b";
    dictionary_[SDLK_c]             = "key_c";
    dictionary_[SDLK_d]             = "key_d";
    dictionary_[SDLK_e]             = "key_e";
    dictionary_[SDLK_f]             = "key_f";
    dictionary_[SDLK_g]             = "key_g";
    dictionary_[SDLK_h]             = "key_h";
    dictionary_[SDLK_i]             = "key_i";
    dictionary_[SDLK_j]             = "key_j";
    dictionary_[SDLK_k]             = "key_k";
    dictionary_[SDLK_l]             = "key_l";
    dictionary_[SDLK_m]             = "key_m";
    dictionary_[SDLK_n]             = "key_n";
    dictionary_[SDLK_o]             = "key_o";
    dictionary_[SDLK_p]             = "key_p";
    dictionary_[SDLK_q]             = "key_q";
    dictionary_[SDLK_r]             = "key_r";
    dictionary_[SDLK_s]             = "key_s";
    dictionary_[SDLK_t]             = "key_t";
    dictionary_[SDLK_u]             = "key_u";
    dictionary_[SDLK_v]             = "key_v";
    dictionary_[SDLK_w]             = "key_w";
    dictionary_[SDLK_x]             = "key_x";
    dictionary_[SDLK_y]             = "key_y";
    dictionary_[SDLK_z]             = "key_z";
}

/*
std::string Input::LookUp(uint index)
{
    std::string s = Input::GetInstance()->dictionary_[index];
    if (s == "")
    {
        std::cout << "Warning: Index " << index << " is not defined in dictionary." << std::endl;
        return "";
    }
    return s;
}

std::string Input::LookUp(std::map<uint, std::string>::iterator it)
{
    return Input::GetInstance()->dictionary_[it->first];
}

*/
