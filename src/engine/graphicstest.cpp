#include "graphicstest.h"
#include <OGRE/Ogre.h>
#include <SDL2/SDL_syswm.h>
#include "common.h"
#ifdef __APPLE__
#include "macosx.h"
#endif
#include "gametest.h"

using namespace Ogre;
using namespace std;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static string WINDOW_TITLE = "Velonaut";

GraphicsTest::GraphicsTest() {}

void GraphicsTest::init()
{
    assert(!SDL_Init(SDL_INIT_EVERYTHING));

    root_ = new Root();
    assert(root_);

#ifdef __APPLE__
    root_->loadPlugin("RenderSystem_GL");
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

    createScene();
}

void GraphicsTest::render()
{
    root_->_fireFrameStarted();
    root_->renderOneFrame();
    root_->_fireFrameRenderingQueued();
    root_->_fireFrameEnded();

#ifndef __APPLE__
    SDL_GL_SwapWindow(window_);
#endif
}

void GraphicsTest::shutdown()
{
    delete root_;

    SDL_DestroyWindow(window_);
    SDL_Quit();
}

GraphicsTest *GraphicsTest::GetInstance()
{
    return GameTest::GetApp()->GetGraphics();
}

void GraphicsTest::initWindow()
{
    window_ = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_HIDDEN
                               | SDL_WINDOW_OPENGL
                               | SDL_WINDOW_RESIZABLE);
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

    renderWindow_ = root_->createRenderWindow(WINDOW_TITLE,
                                              WINDOW_WIDTH,
                                              WINDOW_HEIGHT,
                                              false,
                                              &params);

    // Both calls are needed
    SDL_ShowWindow(window_);
    renderWindow_->setVisible(true);
}

void GraphicsTest::initResources()
{
    // For now just using the default resource group for simplicity
    ResourceGroupManager& resGroupManager = ResourceGroupManager::getSingleton();
    resGroupManager.addResourceLocation("data/meshes", "FileSystem");
    resGroupManager.addResourceLocation("data/materials", "FileSystem");
    resGroupManager.initialiseAllResourceGroups();
    resGroupManager.loadResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void GraphicsTest::createScene()
{
    scene_ = root_->createSceneManager(Ogre::ST_GENERIC);
    Camera* camera = scene_->createCamera("cam");

    Viewport* vp = renderWindow_->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0.0, 0.0, 0.1));

    camera->setAspectRatio((float)WINDOW_WIDTH / WINDOW_HEIGHT);

    scene_->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
    scene_->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    //scene_->setFog(FOG_LINEAR,ColourValue::Black,0.00001,0,10000);

    camNode_ = scene_->getRootSceneNode()->createChildSceneNode();

    camNode_->setPosition(Vector3(0, 0, 10));
    camNode_->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);
    camera->setNearClipDistance(0.001);
    camera->setFarClipDistance(1000);

    camNode_->attachObject(camera);
    camera->lookAt(0,0,-1);

//    Ogre::Light* pointLight = scene_->createLight();
//    pointLight->setType(Ogre::Light::LT_POINT);
//    pointLight->setPosition(Ogre::Vector3(0, 0, 2));
//    pointLight->setDiffuseColour(0.7, 0.7, 1.0);
}

Ogre::SceneNode* GraphicsTest::createEllipsoid(std::vector<double> position, std::vector<double> orientation, std::vector<double> scale)
{
    Entity* entity = GraphicsTest::GetInstance()->scene_->createEntity(Ogre::SceneManager::PT_SPHERE);
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setReceiveShadows(true);
    entity->setMaterial(material);
    entity->setCastShadows(true);
    SceneNode* node = GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(position[0], position[1], position[2]);
    node->setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
    node->scale(scale[0]*0.01, scale[1]*0.01, scale[2]*0.01);
    return node;
}

Ogre::SceneNode* GraphicsTest::createBox(std::vector<double> position, std::vector<double> orientation, std::vector<double> scale)
{
    Entity* entity = GraphicsTest::GetInstance()->scene_->createEntity(Ogre::SceneManager::PT_CUBE);
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setReceiveShadows(true);
    entity->setMaterial(material);
    entity->setCastShadows(true);
    SceneNode* node = GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(position[0], position[1], position[2]);
    node->setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
    node->scale(scale[0]*0.01, scale[1]*0.01, scale[2]*0.01);
    return node;
}

Ogre::SceneNode* GraphicsTest::createPlane(std::vector<double> position, std::vector<double> orientation)
{
    Plane plane(Ogre::Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                           1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
    Entity* entity = GraphicsTest::GetInstance()->scene_->createEntity("plane");
    MaterialPtr material = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setReceiveShadows(true);
    entity->setMaterial(material);
    entity->setCastShadows(true);
    SceneNode* node = GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(position[0], position[1], position[2]);
    node->setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
    return node;
}

std::vector<double> GraphicsTest::getSceneNodePosition(Ogre::SceneNode* node)
{
    Vector3 pos = node->getPosition();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = pos.x;
    ret[1] = pos.y;
    ret[2] = pos.z;
    return ret;
}

void GraphicsTest::setSceneNodePosition(Ogre::SceneNode* node, std::vector<double> pos)
{
    node->setPosition(Vector3(pos[0], pos[1], pos[2]));
}

std::vector<double> GraphicsTest::getSceneNodeOrientation(Ogre::SceneNode* node)
{
    Quaternion ori = node->getOrientation();
    std::vector<double> ret = std::vector<double>(4);
    ret[0] = ori.w;
    ret[1] = ori.x;
    ret[2] = ori.y;
    ret[3] = ori.z;
    return ret;
}

void GraphicsTest::setSceneNodeOrientation(Ogre::SceneNode* node, std::vector<double> ori)
{
    node->setOrientation(Quaternion(ori[0], ori[1], ori[2], ori[3]));
}

std::vector<double> GraphicsTest::getCameraPosition()
{
    std::vector<double> ret = std::vector<double> (3);
    Ogre::Vector3 position = GraphicsTest::GetInstance()->camNode_->getPosition();
    ret[0] = position.x;
    ret[1] = position.y;
    ret[2] = position.z;
    return ret;
}

void GraphicsTest::setCameraPostion(std::vector<double> position)
{
    GraphicsTest::GetInstance()->camNode_->setPosition(position[0], position[1], position[2]);
}

void GraphicsTest::setCameraOrientation(std::vector<double> orientation)
{
    Ogre::Quaternion orientationQua = Ogre::Quaternion(orientation[0], orientation[1], orientation[2], orientation[3]);
    GraphicsTest::GetInstance()->camNode_->setOrientation(orientationQua);
}

std::vector<double> GraphicsTest::getCameraDerivedDirection()
{
    Camera* camera = (Camera*)GraphicsTest::GetInstance()->camNode_->getAttachedObject("cam");
    std::vector<double> ret = std::vector<double>(3);
    if (camera)
    {
        Ogre::Vector3 direction = camera->getDerivedDirection();
        ret[0] = direction.x;
        ret[1] = direction.y;
        ret[2] = direction.z;
        return ret;
    }
    return ret;
}

std::vector<double> GraphicsTest::getCameraDerivedUp()
{
    Camera* camera = (Camera*)GraphicsTest::GetInstance()->camNode_->getAttachedObject("cam");
    Ogre::Vector3 direction = camera->getDerivedUp();
    std::vector<double> ret = std::vector<double>(3);
    ret[0] = direction.x;
    ret[1] = direction.y;
    ret[2] = direction.z;
    return ret;
}

void GraphicsTest::createTunnel(unsigned int seed)
{
    // PARAMS
    int numCurves = 8;
    int samplesPerCurve = 100;
    int ringSamples = 30;
    double minControlRad = 8;
    double maxControlRad = 10;
    double minAnchorRad = 8;
    double maxAnchorRad = 10;
    double ringRadius = 6;
    double looseningTerm = 1;
    double loosen; double oldLength;
    unsigned  int testSeed = 774265562;

    srand(testSeed);
    std::vector<BezierTest> curves = std::vector<BezierTest>(numCurves);

    // TODO: REMOVE TESTING STUFF
    std::vector<double> size = std::vector<double> (3);
    size[0] = 0.01; size[1] = 0.01; size[2] = 0.01;
//    std::vector<double> derSize = std::vector<double> (3);
//    derSize[0] = 0.01; derSize[1] = 0.01; derSize[2] = 0.01;
    std::vector<double> ori = std::vector<double> (4);
    ori[0] = 1; ori[1] = 0; ori[2] = 0; ori[3] = 0;
    std::vector<double> pos = std::vector<double>(3);
//    std::vector<double> tanRefPos = std::vector<double>(3);
//    std::vector<double> norRefPos = std::vector<double>(3);

    // FIRST CURVE
    Ogre::Vector3 p0 = Ogre::Vector3(0,0,0);
    Ogre::Vector3 p1 = Ogre::Vector3(0,0,-1);
    double r_p1 = (double) rand() / RAND_MAX;
    r_p1 = minControlRad + r_p1 * (maxControlRad - minControlRad);
    p1 *= r_p1;
    // Ensure that first control point is on xz plane,
    // thus making first normal the up vector
    if (p1.x < 0.0001 && p1.z < 0.0001)
        p1.x = minControlRad;
    p1.y = 0;
    Ogre::Vector3 p2 = p1 + getRandomPoint(minControlRad, maxControlRad);
    Ogre::Vector3 p3 = p2 + getRandomPoint(minAnchorRad, maxAnchorRad);
    loosen = looseningTerm*((2*M_PI)-((p0-p1).angleBetween(p3-p2)).valueRadians());
    oldLength = p1.length(); p1 = p1.normalisedCopy()*oldLength*loosen;
    oldLength = p2.length(); p2 = p2.normalisedCopy()*oldLength*loosen;
    BezierTest first = BezierTest(p0, p1, p2, p3);
    curves[0] = first;

    // MIDDLE OF SPLINE
    for (int i = 1; i < numCurves-1; i++)
    {
        p0 = curves[i-1].p3_;
        p1 = p0 - curves[i-1].p2_;
        p1.normalise();
        r_p1 = (double) rand() / RAND_MAX;
        r_p1 = minControlRad + r_p1 * (maxControlRad - minControlRad);
        p1 *= r_p1;
        p1 = p0 + p1;
        Ogre::Vector3 a2p = curves[i-1].p2_-curves[i-1].p3_;
        Ogre::Vector3 a2c = p1-p0;
        p2 = p1 + getRandomPoint(minControlRad, maxControlRad);
        p3 = p2 + getRandomPoint(minAnchorRad, maxAnchorRad);
        /*
        if ((double)i/numCurves >= goHome)
        {
            double curWeight = ((double)(i/numCurves) - goHome)/(1-goHome);
            double endWeight = 1 - curWeight;
            p3 = ((curWeight) * p3) + ((endWeight) * first.p0_);
        }
        */
        loosen = looseningTerm*((2*M_PI)-((p0-p1).angleBetween(p3-p2)).valueRadians());
        oldLength = p1.length(); p1 = p1.normalisedCopy()*oldLength*loosen;
        oldLength = p2.length(); p2 = p2.normalisedCopy()*oldLength*loosen;
        curves[i] = BezierTest(p0, p1, p2, p3);
    }

    // LAST CURVE
    p0 = curves[numCurves-2].p3_;
    p1 = p0 - curves[numCurves-2].p2_;
    p1.normalise();
    double r = (double) rand() / RAND_MAX;
    r = minControlRad + r * (maxControlRad - minControlRad);
    p1 *= r;
    p1 = p0 + p1;
    p3 = first.p0_;
    p2 = p3 - first.p1_;
    p2.normalise();
    p2 *= (p3-p1).length() + (p1-p1).length();
    p2 = p3 + p2;
    BezierTest last = BezierTest(p0, p1, p2, p3);
    curves[numCurves-1] = last;



    // CREATE SAMPLES
    Ogre::Vector3 normalHelper = Ogre::Vector3(0,1,0);
    Ogre::Vector3 lastNormal = Ogre::Vector3(0,1,0);
    std::vector<Ogre::Vector3> samples = std::vector<Ogre::Vector3>(numCurves*samplesPerCurve*ringSamples);
    std::vector<Ogre::Vector3> curve = std::vector<Ogre::Vector3>(numCurves*samplesPerCurve);
    int vertCount = 0;
    int curvCount = 0;
    for (int i = 0; i < numCurves; i++)
    {
        BezierTest b = curves[i];
        for (double j = 0; j < 1; j += (double)(1.0/samplesPerCurve))
        {
            Ogre::Vector3 point = b.getPoint(j);
            Ogre::Vector3 tangent = b.getDerPoint(j).normalisedCopy();
            Ogre::Vector3 normal = tangent.crossProduct(normalHelper).normalisedCopy();
            if (normal.x < 0.0001 && normal.y < 0.0001 && normal.z < 0.0001)
                normal = lastNormal;
            normalHelper = normal.crossProduct(tangent);
            lastNormal = normal;
            curve[curvCount] = point; curvCount++;
            for (int k = 0; k < ringSamples; k++)
            {
                double theta = k*2*M_PI/ringSamples;
                Ogre::Vector3 sample = point + (Math::Cos(theta)*ringRadius*normal) +
                        (Math::Sin(theta)*ringRadius*(tangent.crossProduct(normal).normalisedCopy()));
                samples[vertCount] = sample; vertCount++;
                //pos[0] = sample.x; pos[1] = sample.y; pos[2] = sample.z;
                //GraphicsTest::GetInstance()->createEllipsoid(pos, ori, size);
            }
//            Ogre::Vector3 tanRefPoint = point + (0.01 * tangent);
//            Ogre::Vector3 norRefPoint = point + (0.01 * normal);
//            pos[0] = point.x; pos[1] = point.y; pos[2] = point.z;
//            tanRefPos[0] = tanRefPoint.x; tanRefPos[1] = tanRefPoint.y; tanRefPos[2] = tanRefPoint.z;
//            norRefPos[0] = norRefPoint.x; norRefPos[1] = norRefPoint.y; norRefPos[2] = norRefPoint.z;
//            GraphicsTest::GetInstance()->createEllipsoid(pos, ori, size);
//            GraphicsTest::GetInstance()->createEllipsoid(tanRefPos, ori, derSize);
//            GraphicsTest::GetInstance()->createEllipsoid(norRefPos, ori, derSize);

        }
    }

    // CREATE MESH
    ManualObject* tunnel = GraphicsTest::GetInstance()->scene_->createManualObject("tunnel");
    tunnel->begin("BaseWhite", RenderOperation::OT_TRIANGLE_STRIP);
    int numRings = numCurves*samplesPerCurve;
    for (int ringIndex = 0; ringIndex < numRings; ringIndex++)
    {
        for (int ringSample = 0; ringSample < ringSamples; ringSample++)
        {
            tunnel->position(samples[(ringIndex*ringSamples)+ringSample]);
            Ogre::Vector3 n = samples[(ringIndex*ringSamples)+ringSample]-curve[ringIndex];
            //std::cout << n << std::endl;
            tunnel->normal(1*n);
        }
    }
    for (int ringIndex = 0; ringIndex < numRings; ringIndex++)
    {
        int currRingSampleBase = (ringIndex*ringSamples);
        int nextRingSampleBase = (currRingSampleBase+ringSamples)%vertCount;
        for (int j = 0; j < ringSamples; j++)
        {
            tunnel->index(nextRingSampleBase+j);
            tunnel->index(currRingSampleBase+j);
        }
    }

    tunnel->end();

    ManualObject* tunnelMesh = GraphicsTest::GetInstance()->scene_->createManualObject("tunnelMesh");
    tunnelMesh->begin("plane", RenderOperation::OT_LINE_STRIP);
    numRings = numCurves*samplesPerCurve;
    for (int ringIndex = 0; ringIndex < numRings; ringIndex++)
    {
        for (int ringSample = 0; ringSample < ringSamples; ringSample++)
        {
            tunnelMesh->position(samples[(ringIndex*ringSamples)+ringSample]);
            Ogre::Vector3 n = samples[(ringIndex*ringSamples)+ringSample]-curve[ringIndex];
            //std::cout << n << std::endl;
            tunnelMesh->normal(-1*n);
        }
    }
    for (int ringIndex = 0; ringIndex < numRings; ringIndex++)
    {
        int currRingSampleBase = (ringIndex*ringSamples);
        int nextRingSampleBase = (currRingSampleBase+ringSamples)%vertCount;
        for (int j = 0; j < ringSamples; j++)
        {
            tunnelMesh->index(nextRingSampleBase+j);
            tunnelMesh->index(currRingSampleBase+j);
        }
    }

    tunnelMesh->end();

    // CREATE PAT
    ManualObject* path = GraphicsTest::GetInstance()->scene_->createManualObject("path");
    path->begin("plane", RenderOperation::OT_LINE_STRIP);
    for (int i = 0; i < curvCount; i++)
    {
        path->position(curve[i]);
        path->index(i);
    }
    path->end();

    GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode()->attachObject(tunnel);
    GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode()->attachObject(tunnelMesh);
    GraphicsTest::GetInstance()->scene_->getRootSceneNode()->createChildSceneNode()->attachObject(path);

}

Ogre::Vector3 GraphicsTest::getRandomPoint(double min, double max)
{
    double x = (double) (rand()-(RAND_MAX/2)) / RAND_MAX;
    double y = (double) (rand()-(RAND_MAX/2)) / RAND_MAX;
    double z = (double) (rand()-(RAND_MAX/2)) / RAND_MAX;
    Ogre::Vector3 dir = Ogre::Vector3(x, y, z);
    dir.normalise();
    double r = (double) rand() / RAND_MAX;
    r = min + r * (max - min);
    dir = dir * r;
    std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
    return dir * r;
}
