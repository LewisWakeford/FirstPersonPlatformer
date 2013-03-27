#include "game.h"

#include <iostream>
#include <exception>

#include "App.h"
#include "Renderer.h"
#include "CollisionSystem.h"
#include "ResourceManager.h"
#include "FloatingCamera.h"
#include "MeshNode.h"
#include "SceneNode.h"
#include "SceneGraph.h"
#include "MapManager.h"
#include "Jumpman.h"
#include "Crosshair.h"

App theApp;

void GLFWCALL keyCallback(int key, int action)
{
    theApp.keyCallback(key, action);
}

void GLFWCALL mouseCallback(int button, int action)
{
    theApp.mouseCallback(button, action);
}

int main()
{
    try{

        int     width, height;
        bool    running = true;

        if(GL_TRUE != glfwInit())
        {
            return 0;
        }

        //Use OpenGL 3.3 at least
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);

        if( !glfwOpenWindow(1280, 720, 0, 0, 0, 0, 0, 0, GLFW_WINDOW) )
        {
            glfwTerminate();
            return 0;
        }
        glfwGetWindowSize(&width, &height);

        checkError();


        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if(GLEW_OK != err)
        {
            return 0;
        }

        checkError();

        //App setup
        theApp.init();
        checkError();

        glfwSetKeyCallback(keyCallback);
        glfwSetMouseButtonCallback(mouseCallback);

        theApp.setFrameFrequency(1.0/60.0);
        theApp.setTickFrequency(1.0/120.0);
        Renderer* renderer = theApp.getRenderer();
        SceneGraph* sceneGraph = theApp.getSceneGraph();
        sceneGraph->setRoot(SceneNodePtr(new SceneNode(&theApp, GAME_RENDER_NONE)));

        checkError();

        //Shader setup
        theApp.getResourceManager()->createProgramFromFiles("block", "shader/block.vert", "shader/block.frag"); checkError();
        theApp.getResourceManager()->createProgramFromFiles("generic_mesh", "shader/generic_mesh.vert", "shader/generic_mesh.frag");
        theApp.getResourceManager()->createProgramFromFiles("generic_ui", "shader/generic_ui.vert", "shader/generic_ui.frag");

        theApp.getMapManager()->load("map/example.map");checkError();
        theApp.getMapManager()->buildMap();checkError();
        theApp.getMapManager()->saveMap();checkError();

        renderer->setDimensions((GLdouble)width, (GLdouble)height);

        OrientationCamera* FPCamera = new OrientationCamera(&theApp, GAME_RENDER_GEOMETRY, 90.0f, 0.1f, 1000.0f);
        SceneNodePtr jumpmanPtr(new Jumpman(&theApp, FPCamera));
        theApp.setRegular((Jumpman*)jumpmanPtr.get(), FPCamera);

        SceneNodePtr cameraPtr(new FloatingCamera(&theApp, GAME_RENDER_GEOMETRY, 90.0f, 0.1f, 1000.0f));
        ((FloatingCamera*)cameraPtr.get())->yaw(3.1f);
        theApp.setEditor((FloatingCamera*)cameraPtr.get(), (FloatingCamera*)cameraPtr.get());

        theApp.useRegular();

        sceneGraph->getRoot()->addChild(cameraPtr);
        sceneGraph->getRoot()->addChild(jumpmanPtr);

        Mesh* debugMesh = theApp.getResourceManager()->getMesh("debug_cube");
        SceneNodePtr meshPtr(new MeshNode(&theApp, GAME_RENDER_GEOMETRY, debugMesh));
        sceneGraph->addNode(meshPtr);

        SceneNodePtr crosshair(new Crosshair(&theApp, width/2, height/2, 10.0f, 1.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
        sceneGraph->addNode(crosshair);

        checkError();

        //End app setup
        glEnable(GL_DEPTH_TEST);checkError();
        glEnable(GL_CULL_FACE);checkError();

        //I basically messed up and had all my matrixes inverted, so all my geometry is wound the wrong way now...
        glfwDisable(GLFW_MOUSE_CURSOR);
        glFrontFace(GL_CCW);checkError();
        glCullFace(GL_BACK);checkError();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);checkError();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);checkError();

        glfwSetWindowTitle("Game");checkError();

        checkError();

        unsigned int frames = 0;
        double lastFPSUpdate = 0.0;
        double elapsedTime = 0.0;

        while(running)
        {
            GLdouble timeToWait = theApp.sleepTime();
            GLdouble deltaTime = theApp.deltaTime();

            //Simulation code
            //if(theApp.timeToSimulate())
            //{
                theApp.processInput();
                sceneGraph->onLoop(deltaTime);
                theApp.getCollisionSystem()->checkCollisions();
                theApp.clockTick();
            //}

            //Rendering code
            //if(theApp.timeToRender())
            //{
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                sceneGraph->onRender(GAME_RENDER_GEOMETRY);
                glClear(GL_DEPTH_BUFFER_BIT);
                sceneGraph->onRender(GAME_RENDER_UI_2D);
                glfwSwapBuffers();
                theApp.clockFrame();
                frames++;
            //}

            elapsedTime = glfwGetTime();

            if(elapsedTime-lastFPSUpdate >= 1.0)
            {
                double fps = frames/(elapsedTime-lastFPSUpdate);
                lastFPSUpdate = elapsedTime;
                frames = 0;
                std::cout << "FPS: " << fps << std::endl;
            }

            // exit if ESC was pressed or window was closed
            running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

            if(running) checkError();

        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    glfwTerminate();

    return 0;

}
