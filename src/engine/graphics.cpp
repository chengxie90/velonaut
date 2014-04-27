#include "graphics.h"
#include <OGRE/Ogre.h>
#include <SDL2/SDL_syswm.h>
#ifdef __APPLE__
#include "macosx.h"
#endif
#include "luamanager.h"
#include "app.h"

using namespace Ogre;
using namespace std;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static string WINDOW_TITLE = "Velonaut";

Graphics::Graphics() {}

void Graphics::init()
{    
    assert(!SDL_Init(SDL_INIT_EVERYTHING));
    
    root_ = new Root();
    assert(root_);
    
#ifdef __APPLE__
    root_->loadPlugin("RenderSystem_GL");
    root_->loadPlugin("Plugin_ParticleFX");
#elif __LINUX__
    // TODO: fix Linux path problem
    root_->loadPlugin("./Linux/Debug/RenderSystem_GL.so");
#endif
    
    RenderSystem* renderer = root_->getAvailableRenderers()[0];
    assert(renderer);
    root_->setRenderSystem(renderer);
    
    root_->initialise(false);
        
    initWindow();
    initResources();
            
    viewport_ = renderWindow_->addViewport(NULL);
}

void Graphics::shutdown()
{
    meshMap_.clear();
    materialMap_.clear();
    
    delete root_;
    
    SDL_DestroyWindow(window_);    
    SDL_Quit();
}

void Graphics::initLua()
{
    LuaManager::GetInstance()->requiref("engine.graphics.c",[](lua_State* state) {
        luaL_Reg reg[] = {
            {"setActiveScene", Graphics::lsetActiveScene},
            {"setBackgroundColor", Graphics::lsetBackgroundColor},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.scene.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Scene::lcreate},
            {"setMainCamera", Graphics::Scene::lsetMainCamera},
            {"setAmbientLight", Graphics::Scene::lsetAmbientLight},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.node.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Node::lcreate},
            {"setParent", Graphics::Node::lsetParent},
            {"position", Graphics::Node::lposition},
            {"setPosition", Graphics::Node::lsetPosition},
            {"orientation", Graphics::Node::lorientation},
            {"setOrientation", Graphics::Node::lsetOrientation},
            {"attachObject", Graphics::Node::lattachObject},
            {"detachObject", Graphics::Node::ldetachObject},
            {"lookAt", Graphics::Node::llookAt},
            {"localX", Graphics::Node::llocalX},
            {"localY", Graphics::Node::llocalY},
            {"localZ", Graphics::Node::llocalZ},
            {"setScale", Graphics::Node::lsetScale},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.camera.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Camera::lcreate},
            {"setNear", Graphics::Camera::lsetNear},
            {"setFar", Graphics::Camera::lsetFar},
            {"setFOV", Graphics::Camera::lsetFOV},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.light.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Light::lcreate},
            {"setType", Graphics::Light::lsetType},
            {"setDiffuse", Graphics::Light::lsetDiffuse},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.entity.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"createMesh", Graphics::Entity::lcreateMesh},
            {"createSphere", Graphics::Entity::lcreateSphere},
            {"createBox", Graphics::Entity::lcreateBox},
            {"setMaterial", Graphics::Entity::lsetMaterial},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.mesh.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Mesh::lcreate},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.material.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Material::lcreate},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.meshbuilder.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::MeshBuilder::lcreate},
            {"position", Graphics::MeshBuilder::lposition},
            {"normal", Graphics::MeshBuilder::lnormal},
            {"index", Graphics::MeshBuilder::lindex},
            {"getMesh", Graphics::MeshBuilder::lgetMesh},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });

    LuaManager::GetInstance()->requiref("engine.graphics.vqhelper.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"getQuaternionFromAxes", Graphics::VQHelper::lgetQuaternionFromAxes},
            {"getQuaternionFromAngleAxis", Graphics::VQHelper::lgetQuaternionFromAngleAxis},
            {"angleBetween", Graphics::VQHelper::langleBetween},
            {"rotationTo", Graphics::VQHelper::lrotationTo},
            {"rotateBy", Graphics::VQHelper::lrotationTo},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
    
    LuaManager::GetInstance()->requiref("engine.graphics.particle.c", [](lua_State* state) {
        luaL_Reg reg[] = {
            {"create", Graphics::Particle::lcreate},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    });
}

void Graphics::render()
{
    root_->_fireFrameStarted();
    root_->renderOneFrame();
    root_->_fireFrameRenderingQueued();
    root_->_fireFrameEnded();

#ifndef __APPLE__
    SDL_GL_SwapWindow(window_);
#endif
}

Graphics *Graphics::GetInstance()
{
    return App::GetApp()->GetGraphics();
}

void Graphics::initWindow()
{   
    window_ = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_HIDDEN 
                               | SDL_WINDOW_OPENGL 
                               | SDL_WINDOW_RESIZABLE
                               //| SDL_WINDOW_FULLSCREEN_DESKTOP
                               );
    assert(window_);
    
    // Apple's GL context is managed by Ogre
#ifndef __APPLE__
    SDL_GLContext glcontext = NULL;
    glcontext = SDL_GL_CreateContext(window_);
    assert(glcontext);
#endif

    NameValuePairList params;
#ifdef __APPLE__
    params["externalGLControl"] = "1";
    params["externalWindowHandle"] = osx_cocoa_view(window_);
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
#elif __LINUX__
    params["externalGLControl"] = "1";
    params["currentGLContext"] = "1";
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    assert(SDL_GetWindowWMInfo(window_, &info));
    params["parentWindowHandle"] = StringConverter::toString(info.info.x11.window);
#endif
    
#ifndef __APPLE__
    SDL_GL_MakeCurrent(window_, glcontext);
    SDL_GL_SetSwapInterval(1);
#endif
    
    renderWindow_ = root_->createRenderWindow("",
                                              0,
                                              0,
                                              false,
                                              &params);
    
    // Both calls are needed
    SDL_ShowWindow(window_);
    renderWindow_->setVisible(true);
}

void Graphics::initResources()
{
    // For now just using the default resource group for simplicity
    ResourceGroupManager& resGroupManager = ResourceGroupManager::getSingleton();
    resGroupManager.addResourceLocation("data/meshes", "FileSystem");
    resGroupManager.addResourceLocation("data/materials", "FileSystem");
    resGroupManager.addResourceLocation("data/textures", "FileSystem");
    resGroupManager.addResourceLocation("data/particles", "FileSystem");
    resGroupManager.initialiseAllResourceGroups();
    resGroupManager.loadResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

int Graphics::Scene::lcreate(lua_State *)
{
    SceneManager* scene = Graphics::GetInstance()->root_->createSceneManager(Ogre::ST_GENERIC);
    //scene->setSkyBox(true, "skybox2");
    scene->setSkyDome(true, "skybox", 1, 1, 5000, true);
    LuaManager::GetInstance()->addParam((void *)scene);
    return 1;
}

int Graphics::lsetActiveScene(lua_State *)
{
    SceneManager* scene;
    LuaManager::GetInstance()->extractParam((void **)&scene);
    Graphics::GetInstance()->scene_ = scene;
    return 0;
}

int Graphics::lsetBackgroundColor(lua_State *)
{
    ColourValue color;
    LuaManager::GetInstance()->extractParam(&color);
    Graphics::GetInstance()->viewport_->setBackgroundColour(color);
    return 0;
}

int Graphics::Camera::lcreate(lua_State *)
{
    SceneManager* scene = Graphics::GetInstance()->scene_;
    string name;
    LuaManager::GetInstance()->extractParam(&name);
    Ogre::Camera* camera = scene->createCamera(name);
    float width = Graphics::GetInstance()->renderWindow_->getWidth();
    float height = Graphics::GetInstance()->renderWindow_->getHeight();
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.f);
    camera->setAspectRatio(width / height);
    LuaManager::GetInstance()->addParam((void*)camera);
    return 1;
}

int Graphics::Scene::lsetMainCamera(lua_State *)
{
    Ogre::Camera* camera;
    LuaManager::GetInstance()->extractParam((void **)&camera);
    assert(camera);
    Graphics::GetInstance()->viewport_->setCamera(camera);
    return 0;
}

int Graphics::Light::lcreate(lua_State *)
{
    Ogre::Light* light = Graphics::GetInstance()->scene_->createLight();
    LuaManager::GetInstance()->addParam((void *)light);
    return 1;
}

int Graphics::Scene::lsetAmbientLight(lua_State *)
{
    ColourValue color;
    LuaManager::GetInstance()->extractParam(&color);
    Graphics::GetInstance()->scene_->setAmbientLight(color);
    return 0;
}

int Graphics::Entity::lcreateMesh(lua_State *)
{
    Ogre::Mesh* pmesh = NULL;
    LuaManager::GetInstance()->extractParam((void **)&pmesh);
        
    MeshPtr mesh = Graphics::GetInstance()->meshMap_[pmesh];
    
    Ogre::Entity* entity = Graphics::GetInstance()->scene_->createEntity(mesh);
    
    LuaManager::GetInstance()->addParam((void *)entity);
    
    return 1;
}

int Graphics::Entity::lcreateSphere(lua_State *)
{
    Ogre::Entity* entity = Graphics::GetInstance()->scene_->createEntity(Ogre::SceneManager::PT_SPHERE);

    LuaManager::GetInstance()->addParam((void *)entity);

    return 1;
}

int Graphics::Entity::lcreateBox(lua_State *)
{
    Ogre::Entity* entity = Graphics::GetInstance()->scene_->createEntity(Ogre::SceneManager::PT_CUBE);

    LuaManager::GetInstance()->addParam((void *)entity);

    return 1;
}

int Graphics::Entity::lsetMaterial(lua_State *)
{
    Ogre::Entity* entity;
    LuaManager::GetInstance()->extractParam((void **)&entity);
    
    Ogre::Material* pmat = NULL;
    LuaManager::GetInstance()->extractParam((void **)&pmat);

    MaterialPtr mat = Graphics::GetInstance()->materialMap_[pmat];
    
    entity->setMaterial(mat);
    
    return 0;
}

int Graphics::Node::lcreate(lua_State *)
{
    SceneNode* node = Graphics::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    LuaManager::GetInstance()->addParam((void *)node);
    return 1;
}

int Graphics::Node::lsetParent(lua_State *)
{
    SceneNode* parent;
    LuaManager::GetInstance()->extractParam((void**)&parent);
    
    SceneNode* node;
    LuaManager::GetInstance()->extractParam((void**)&node);
    
    node->getParentSceneNode()->removeChild(node);
    parent->addChild(node);
    
    return 0;
}

int Graphics::Node::lposition(lua_State *)
{
    SceneNode *node;
    LuaManager::GetInstance()->extractParam((void **)&node);

    Vector3 pos = node->getPosition();

    LuaManager::GetInstance()->addParam(pos);

    return 1;
}

int Graphics::Node::lsetPosition(lua_State *)
{
    SceneNode *node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    
    Vector3 pos;
    LuaManager::GetInstance()->extractParam(&pos);

    node->setPosition(pos);
    return 0;
}


int Graphics::Node::lorientation(lua_State *)
{
    SceneNode *node;
    LuaManager::GetInstance()->extractParam((void **)&node);

    Quaternion ori = node->getOrientation();

    LuaManager::GetInstance()->addParam(ori);

    return 1;
}


int Graphics::Node::lsetOrientation(lua_State *)
{
    SceneNode *node;
    LuaManager::GetInstance()->extractParam((void **)&node);

    Quaternion ori;
    LuaManager::GetInstance()->extractParam(&ori);

    node->setOrientation(ori);
    return 0;
}

int Graphics::Node::llookAt(lua_State *)
{
    SceneNode *node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    assert(node);
    Vector3 target;
    LuaManager::GetInstance()->extractParam(&target);
    node->lookAt(target, Ogre::Node::TS_WORLD);
    return 0;
}

int Graphics::Node::lattachObject(lua_State *)
{
    SceneNode* node;
    Ogre::MovableObject* obj;
    LuaManager::GetInstance()->extractParam((void**)&node);
    LuaManager::GetInstance()->extractParam((void**)&obj);
    assert(node);
    assert(obj);
    node->attachObject(obj);
    return 0;
}

int Graphics::Node::ldetachObject(lua_State *)
{
    SceneNode* node;
    Ogre::Camera* obj;
    LuaManager::GetInstance()->extractParam((void**)&node);
    LuaManager::GetInstance()->extractParam((void**)&obj);
    assert(node);
    assert(obj);
    node->detachObject(obj);
    return 0;
}

int Graphics::Node::lsetScale(lua_State *)
{
    SceneNode* node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    assert(node);
    Vector3 scale;
    LuaManager::GetInstance()->extractParam(&scale);
    node->scale(scale);
    return 0;
}

int Graphics::Node::llocalX(lua_State *)
{
    SceneNode* node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    assert(node);

    Matrix3 axes = node->getLocalAxes();
    Vector3 look = axes.GetColumn(0); // assume look starts as -z

    LuaManager::GetInstance()->addParam(look);

    return 1;
}

int Graphics::Node::llocalY(lua_State *)
{
    SceneNode* node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    assert(node);

    Matrix3 axes = node->getLocalAxes();
    Vector3 look = axes.GetColumn(1); // assume look starts as -z

    LuaManager::GetInstance()->addParam(look);

    return 1;
}

int Graphics::Node::llocalZ(lua_State *)
{
    SceneNode* node;
    LuaManager::GetInstance()->extractParam((void **)&node);
    assert(node);

    Matrix3 axes = node->getLocalAxes();
    Vector3 look = axes.GetColumn(2); // assume look starts as -z

    LuaManager::GetInstance()->addParam(look);

    return 1;
}

int Graphics::Camera::lsetNear(lua_State *)
{
    Ogre::Camera* cam;
    double n;
    LuaManager::GetInstance()->extractParam((void **)&cam);
    assert(cam);
    LuaManager::GetInstance()->extractParam(&n);
    assert(n > 0);
    cam->setNearClipDistance(n);
    return 0;
}

int Graphics::Camera::lsetFar(lua_State *)
{
    Ogre::Camera* cam;
    double f;
    LuaManager::GetInstance()->extractParam((void **)&cam);
    assert(cam);
    LuaManager::GetInstance()->extractParam(&f);
    assert(f > 0);
    cam->setFarClipDistance(f);
    return 0;
}

int Graphics::Camera::lsetFOV(lua_State *)
{
    Ogre::Camera* cam;
    double fov;
    LuaManager::GetInstance()->extractParam((void **)&cam);
    assert(cam);
    LuaManager::GetInstance()->extractParam(&fov);
    assert(fov > 0);
    cam->setFOVy(Radian(Degree(fov)));
    return 0;
}

int Graphics::Light::lsetType(lua_State *)
{
    Ogre::Light* light;
    LuaManager::GetInstance()->extractParam((void **)&light);
    assert(light);
    string type;
    LuaManager::GetInstance()->extractParam(&type);
    if (type == "pointlight") {
        light->setType(Ogre::Light::LT_POINT);
    }
    else if (type == "directlight") {
        light->setType(Ogre::Light::LT_DIRECTIONAL);
    }
    else if (type == "spotlight") {
        light->setType(Ogre::Light::LT_SPOTLIGHT);
    }
    else {
        assert(false);
    }
    return 0;
}

int Graphics::Light::lsetDiffuse(lua_State *)
{
    Ogre::Light* light;
    LuaManager::GetInstance()->extractParam((void **)&light);
    assert(light);

    ColourValue color;
    LuaManager::GetInstance()->extractParam(&color);
    
    light->setDiffuseColour(color);
    return 0;
}


int Graphics::Mesh::lcreate(lua_State *)
{
    string name;
    LuaManager::GetInstance()->extractParam(&name);
    
    name += ".mesh";
    
    MeshPtr mesh = MeshManager::getSingleton().create(name, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    Ogre::Mesh* pmesh = mesh.get();
    
    Graphics::GetInstance()->meshMap_[pmesh] = mesh;
    
    LuaManager::GetInstance()->addParam((void *)pmesh);
    return 1;
}


int Graphics::Material::lcreate(lua_State *)
{
    string name;
    LuaManager::GetInstance()->extractParam(&name);
    
    //name += ".material";
    
    MaterialPtr mat = MaterialManager::getSingleton().getByName(name, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    Ogre::Material* pmat = mat.get();
    
    Graphics::GetInstance()->materialMap_[pmat] = mat;
    
    LuaManager::GetInstance()->addParam((void *)pmat);
    
    return 1;
}


int Graphics::MeshBuilder::lcreate(lua_State *)
{
    ManualObject* mo = Graphics::GetInstance()->scene_->createManualObject();
    
    string primitive;
    LuaManager::GetInstance()->extractParam(&primitive);
    
    if (primitive == "TriangleStrip") {
        mo->begin("BaseWhite", RenderOperation::OT_TRIANGLE_STRIP);
    }
    else if (primitive == "LineStrip") {
        mo->begin("BaseWhite", RenderOperation::OT_LINE_STRIP);
    }
    else {
        assert(false);
    }
    
    LuaManager::GetInstance()->addParam((void *)mo);
    
    return 1;
    
}

int Graphics::MeshBuilder::lposition(lua_State *)
{
    ManualObject* mo;
    LuaManager::GetInstance()->extractParam((void **)&mo);
    
    Ogre::Vector3 pos;
    LuaManager::GetInstance()->extractParam(&pos);
    
    mo->position(pos);
    
    return 0;
}

int Graphics::MeshBuilder::lnormal(lua_State *)
{
    ManualObject* mo;
    LuaManager::GetInstance()->extractParam((void **)&mo);
    
    Ogre::Vector3 n;
    LuaManager::GetInstance()->extractParam(&n);
    
    mo->normal(n);
    
    return 0;
}

int Graphics::MeshBuilder::lindex(lua_State *)
{
    ManualObject* mo;
    LuaManager::GetInstance()->extractParam((void **)&mo);
    
    int index;
    LuaManager::GetInstance()->extractParam(&index);
    
    mo->index(index);
    
    return 0;
}

int Graphics::MeshBuilder::lgetMesh(lua_State *)
{
    static string name = "mesh";
    static int count = 0;
    
    ManualObject* mo;
    LuaManager::GetInstance()->extractParam((void **)&mo);
    
    mo->end();
    
    MeshPtr mesh = mo->convertToMesh(name + std::to_string(count++));
    Ogre::Mesh* p = mesh.get();
    Graphics::GetInstance()->meshMap_[p] = mesh;
    
    LuaManager::GetInstance()->addParam((void *)p);
    
    return 1;
}

int Graphics::VQHelper::lgetQuaternionFromAxes(lua_State *)
{
    Ogre::Vector3 x;
    LuaManager::GetInstance()->extractParam(&x);

    Ogre::Vector3 y;
    LuaManager::GetInstance()->extractParam(&y);

    Ogre::Vector3 z;
    LuaManager::GetInstance()->extractParam(&z);

    Ogre::Quaternion q = Ogre::Quaternion(x, y, z);

    LuaManager::GetInstance()->addParam(q);

    return 1;
}

int Graphics::VQHelper::lgetQuaternionFromAngleAxis(lua_State *)
{
    double angle;
    LuaManager::GetInstance()->extractParam(&angle);

    Ogre::Vector3 axis;
    LuaManager::GetInstance()->extractParam(&axis);

    Ogre::Quaternion q = Ogre::Quaternion(Ogre::Radian(angle), axis);

    LuaManager::GetInstance()->addParam(q);

    return 1;
}

int Graphics::VQHelper::langleBetween(lua_State *)
{
    Ogre::Vector3 v1;
    LuaManager::GetInstance()->extractParam(&v1);

    Ogre::Vector3 v2;
    LuaManager::GetInstance()->extractParam(&v2);

    double angle = (v1.angleBetween(v2)).valueRadians();

    LuaManager::GetInstance()->addParam(angle);

    return 1;
}

int Graphics::Particle::lcreate(lua_State *)
{
    static string name = "particle";
    static int count = 0;
    
    string prefab;
    LuaManager::GetInstance()->extractParam(&prefab);
    
    ParticleSystem* ps = Graphics::GetInstance()->scene_->createParticleSystem(name + std::to_string(count++), prefab);
    
    MovableObject *obj = static_cast<MovableObject*>(ps);

    LuaManager::GetInstance()->addParam((void *)obj);
    
    return 1;
    
}

int Graphics::VQHelper::lrotationTo(lua_State *)
{
    Ogre::Vector3 v1;
    LuaManager::GetInstance()->extractParam(&v1);

    Ogre::Vector3 v2;
    LuaManager::GetInstance()->extractParam(&v2);

    Ogre::Quaternion q = v1.getRotationTo(v2);

    LuaManager::GetInstance()->addParam(q);

    return 1;
}

int Graphics::VQHelper::lrotationBy(lua_State *)
{
    Ogre::Vector3 v;
    LuaManager::GetInstance()->extractParam(&v);

    Ogre::Quaternion q;
    LuaManager::GetInstance()->extractParam(&q);

    Ogre::Vector3 ret = q * v;

    LuaManager::GetInstance()->addParam(ret);

    return 1;
}
