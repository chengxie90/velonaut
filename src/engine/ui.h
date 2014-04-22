#ifndef UI_H
#define UI_H

#include <OGRE/Ogre.h>
#include <Rocket/Core.h>
#include <lua/lua.hpp>
#include "common.h"
#include "inputlistener.h"
#include "rocketeventlistener.h"

class Ui : public Ogre::RenderQueueListener, InputListener
{
public:

    void init();
    void initLua();
    void shutdown();

    virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);

    virtual void onMouseDown(SDL_Event e );
    virtual void onMouseUp(SDL_Event e );
    virtual void onMouseMove(SDL_Event e );

    static Ui* GetInstance();

    Rocket::Core::ElementDocument *doc_;

private:
    SINGLETON(Ui)
    friend class App;
    void configureRenderSystem();

    static int lLoadDocument(lua_State* state);
    static int lLoadMouseCursor(lua_State* state);
    static int lLoadFont(lua_State* state);
    static int lAddEventListener(lua_State* state);

private:
    Rocket::Core::Context* context_;
    Ogre::Matrix4 projection_matrix;
};

#endif // UI_H
