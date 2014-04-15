#ifndef MACOSX_H
#define MACOSX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <OGRE/Ogre.h>
#include <Cocoa/Cocoa.h>

Ogre::String osx_cocoa_view(SDL_Window * window) {
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    assert(SDL_GetWindowWMInfo(window, &info));
    NSWindow* nswindow = info.info.cocoa.window;
    assert(nswindow);
    NSView *view = [nswindow contentView];
    assert(view);
    return Ogre::StringConverter::toString( (unsigned long)view );
}

#endif // MACOSX_H
