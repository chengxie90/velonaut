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
    virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

    virtual void onMouseDown(SDL_Event e );
    virtual void onMouseUp(SDL_Event e );
    virtual void onMouseMove(SDL_Event e );

    static Ui* GetInstance();

    Rocket::Core::ElementDocument *doc_;

private:
    SINGLETON(Ui)
    friend class App;
    void configureRenderSystem();



    Rocket::Core::Context* context_;
    Ogre::Matrix4 projection_matrix;
    static int r;

    static int LSceneLoadDocument(lua_State* state);
    static int LSceneLoadMouseCursor(lua_State* state);
    static int LSceneLoadFont(lua_State* state);
    static int LSceneAddEventListener(lua_State* state);

};

#endif // UI_H
