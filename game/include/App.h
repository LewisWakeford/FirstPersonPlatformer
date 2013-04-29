#ifndef APP_H
#define APP_H

#include "game.h"

#include "LocalHardwareInput.h"
#include "ActionSet.h"
#include "InputMapping.h"

class Renderer;
class ResourceManager;
class SceneGraph;
class CollisionSystem;
class MapManager;
class I_Controllable;
class CameraNode;

class App
{
    public:
        App();
        virtual ~App();

        Renderer* getRenderer();
        ResourceManager* getResourceManager();
		MapManager* getMapManager();
        SceneGraph* getSceneGraph();
        CollisionSystem* getCollisionSystem();

        void setControllable(I_Controllable* controllable);
        void setRegular(I_Controllable* controllable, CameraNode* camera);
        void setEditor(I_Controllable* controllable, CameraNode* camera);
        void useRegular();
        void useEditor();

        //Process input to the current controllable node
        void processInput();

        //Pass along params to input object.
        void keyCallback(int key, int action);
        void mouseCallback(int button, int action);

        //Set the frequnecy of ticks, in seconds.
        void setTickFrequency(GLdouble tickFreq);

        //Set the frequency of frames, in seconds.
        void setFrameFrequency(GLdouble frameFreq);

        //Register that simulation has been done
        void clockTick();

        //Register that rendering has been done
        void clockFrame();
        GLdouble deltaTime();

        //Returns true if it is time to render another frame.
        GLboolean timeToRender();

        //Returns true if it is time to simulate another tick.
        GLboolean timeToSimulate();

        //Get the amount of time the app can sleep.
        GLdouble sleepTime();

        void init();

        double gGravity;

        bool nextMap(); //Returns true when time to load a new level.
        void reachedEndOfLevel(); //Load next map.
        void mapLoaded();

        bool playerHasStarted();
        void waitForInput();


    protected:

        Renderer* mRenderer;
        ResourceManager* mResourceManager;
		MapManager* mMapManager;
        SceneGraph* mSceneGraph;
        CollisionSystem* mCollisionSystem;

        LocalHardwareInput mLocalHardwareInput;

        //Action set for player
        ActionSet mPlayerActionSet;

        //Control scheme for map editor mode.
        InputMapping mMapEditorControl;

        //Control scheme for standard mode.
        InputMapping mStandardControl;

        //The current thing the player is controlling
        I_Controllable* mCurrentControllable;

        GLdouble mLastFrameTime;
        GLdouble mLastTickTime;
        GLdouble mTickFreq;
        GLdouble mFrameFreq;

        GLint mTickCount;
        GLint mFrameCount;

        I_Controllable* mRegularController;
        I_Controllable* mEditorController;
        CameraNode* mRegularCamera;
        CameraNode* mEditorCamera;

        bool mNeedToLoadMap;
        bool mPlayerHasStarted;

    private:
};

#endif // APP_H
