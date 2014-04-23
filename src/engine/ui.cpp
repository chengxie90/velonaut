#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "ui.h"
#include "uirenderinterface.h"
#include "uisysteminterface.h"
#include "app.h"

Ui::Ui()
{
}

void Ui::initLua() {

    LuaManager::GetInstance()->requiref("engine.gui.c",[](lua_State* state) {
        luaL_Reg reg[] = {
            {"loadDocument", Ui::lLoadDocument},
            {"loadCursor", Ui::lLoadMouseCursor},
            {"loadFont", Ui::lLoadFont},
            {"setText", Ui::lSetText},
            {"addEventListener", Ui::lAddEventListener},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    } );
}

void Ui::init() {

   // Set up the projection and view matrices.
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projection_matrix[0][0] = 2.0f / 800;
    projection_matrix[0][3]= -1.0000000f;
    projection_matrix[1][1]= -2.0f / 600;
    projection_matrix[1][3]= 1.0000000f;
    projection_matrix[2][2]= -2.0f / (z_far - z_near);
    projection_matrix[3][3]= 1.0000000f;

    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Rocket");

    Rocket::Core::SetRenderInterface(new UiRenderInterface(800, 600));
    Rocket::Core::SetSystemInterface(new UiSystemInterface());

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    context_ = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(800, 600));
    Rocket::Debugger::Initialise(context_);

    context_->ShowMouseCursor(true);
}

void Ui::shutdown() {
context_->UnloadAllDocuments();
}

int Ui::lLoadDocument(lua_State *state)
{
    std::string doc;
    LuaManager::GetInstance()->extractParam(&doc);
    Ui::GetInstance()->doc_ = Ui::GetInstance()->context_->LoadDocument( doc.c_str() );
    if (Ui::GetInstance()->doc_)
    {
        Ui::GetInstance()->doc_->Show();
        Ui::GetInstance()->doc_->RemoveReference();
    }
}

int Ui::lLoadMouseCursor(lua_State *state)
{
    // Load the mouse cursor and release the caller's reference.
    Rocket::Core::ElementDocument* cursor = Ui::GetInstance()->context_->LoadMouseCursor("data/ui/cursor.rml");
    if (cursor)
        cursor->RemoveReference();
}

int Ui::lAddEventListener(lua_State *state) {

    std::string id;
    std::string event;

    LuaManager::GetInstance()->extractParam(&id);
    LuaManager::GetInstance()->extractParam(&event);

    int r = luaL_ref(state, LUA_REGISTRYINDEX);
    Ui::GetInstance()->doc_->GetElementById(id.c_str())->AddEventListener(event.c_str(), new RocketEventListener(r), true );
}

int Ui::lSetText(lua_State *state) {
    std::string id;
    std::string text;

    LuaManager::GetInstance()->extractParam(&id);
    LuaManager::GetInstance()->extractParam(&text);

    Ui::GetInstance()->doc_->GetElementById(id.c_str())->SetInnerRML(text.c_str());

}


// TODO: Add RemoveEventListener function

Ui *Ui::GetInstance()
{
    return App::GetApp()->GetUi();
}

int Ui::lLoadFont(lua_State *state)
{
    std::string s;
    LuaManager::GetInstance()->extractParam(&s);
    Rocket::Core::FontDatabase::LoadFontFace(s.c_str());
}

void Ui::onMouseDown(SDL_Event e) {
    context_->ProcessMouseButtonDown(0, 0);
}

void Ui::onMouseMove(SDL_Event e) {
    context_->ProcessMouseMove(e.motion.x, e.motion.y, 0);
}

void Ui::onMouseUp(SDL_Event e ) {
    context_->ProcessMouseButtonUp(0, 0);
}

// Called from Ogre before a queue group is rendered.
void Ui::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& ROCKET_UNUSED(skipThisInvocation))
{
    //std::cout << "rendering" << std::endl;
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
    {
        context_->Update();
        configureRenderSystem();
        context_->Render();
    }
}

void Ui::configureRenderSystem()
{
    Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

    render_system->_setProjectionMatrix(projection_matrix);
    render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    render_system->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    render_system->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    render_system->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    render_system->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    render_system->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    render_system->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    render_system->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    render_system->_setDepthBias(0, 0);
}
