#include "App.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneGraph.h"
#include "CollisionSystem.h"
#include "MapManager.h"
#include "I_Controllable.h"

App::App()
{
    gGravity = 10.0f;
}

App::~App()
{
    delete mRenderer;
    delete mResourceManager;
    delete mSceneGraph;
    delete mCollisionSystem;
}

Renderer* App::getRenderer()
{
    return mRenderer;
}

ResourceManager* App::getResourceManager()
{
    return mResourceManager;
}

MapManager* App::getMapManager()
{
    return mMapManager;
}

SceneGraph* App::getSceneGraph()
{
    return mSceneGraph;
}

CollisionSystem* App::getCollisionSystem()
{
    return mCollisionSystem;
}

void App::init()
{
    mRenderer = new Renderer();
    checkError();
    mResourceManager = new ResourceManager();
    checkError();
    mMapManager = new MapManager(this);
    checkError();
    mSceneGraph = new SceneGraph();
    checkError();
    mCollisionSystem = new CollisionSystem(this);
    checkError();
    mFrameCount = 0;
    mTickCount = 0;

    mStandardControl.setBind(GLFW_KEY_SPACE, ACTION_B_JUMP);

    mLocalHardwareInput.setActionSet(&mPlayerActionSet);
    mLocalHardwareInput.setMapping(&mStandardControl);

    //Hacked


    mLastTickTime = glfwGetTime();
    mLastFrameTime = mLastTickTime;
}

void App::setFrameFrequency(GLdouble frameFreq)
{
    mFrameFreq = frameFreq;
}

void App::setTickFrequency(GLdouble tickFreq)
{
    mTickFreq = tickFreq;
}

GLboolean App::timeToRender()
{
    if(mFrameCount < 1)
    {
        return true;
    }
    GLdouble currentTime = glfwGetTime();
    return (currentTime - mLastFrameTime > mFrameFreq);
}

GLboolean App::timeToSimulate()
{
    if(mTickCount < 1)
    {
        return true;
    }
    GLdouble currentTime = glfwGetTime();
    return (currentTime - mLastTickTime > mTickFreq);
}

GLdouble App::sleepTime()
{
    if(mTickCount < 1 || mFrameCount < 1)
    {
        return 0;
    }

    GLdouble currentTime = glfwGetTime();
    GLdouble timeToNextFrame = (mFrameFreq + mLastFrameTime) - currentTime;
    GLdouble timeToNextSim = (mTickFreq + mLastTickTime) - currentTime;

    if(timeToNextFrame < timeToNextSim)
    {
        return timeToNextFrame;
    }
    else
    {
        return timeToNextSim;
    }
}

GLdouble App::deltaTime()
{
    if(mTickCount < 1)
    {
        return 0;
    }
    GLdouble currentTime = glfwGetTime();
    GLdouble delta = currentTime - mLastTickTime;
    mLastTickTime = currentTime;
    return delta;
}

void App::clockTick()
{
    mLocalHardwareInput.updateMouse();
    //mLastTickTime = glfwGetTime();
    mTickCount++;
}

void App::clockFrame()
{
    mLastFrameTime = glfwGetTime();
    mFrameCount++;
}

void App::keyCallback(int key, int action)
{
    if(key == GLFW_KEY_F10 && action == GLFW_RELEASE)
    {
        useRegular();
    }
    else if(key == GLFW_KEY_F11 && action == GLFW_RELEASE)
    {
        useEditor();
    }
    else
    {
        mLocalHardwareInput.keyCallback(key, action);
    }
}

void App::mouseCallback(int button, int action)
{
    mLocalHardwareInput.mouseCallback(button, action);
}

void App::processInput()
{
    if(mCurrentControllable)
    {
        mCurrentControllable->processInput(&mPlayerActionSet);
    }
}

void App::setControllable(I_Controllable* controllable)
{
    mCurrentControllable = controllable;
}

void App::setRegular(I_Controllable* controllable, CameraNode* camera)
{
    mRegularController = controllable;
    mRegularCamera = camera;
}

void App::setEditor(I_Controllable* controllable, CameraNode* camera)
{
    mEditorController = controllable;
    mEditorCamera = camera;
}

void App::useRegular()
{
    setControllable(mRegularController);
    mRenderer->setCamera(mRegularCamera);
    mLocalHardwareInput.setMapping(&mStandardControl);
}

void App::useEditor()
{
    setControllable(mEditorController);
    mRenderer->setCamera(mEditorCamera);
    mLocalHardwareInput.setMapping(&mMapEditorControl);
}
