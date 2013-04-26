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

#include <glm/gtc/type_ptr.hpp>

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
        bool    notStarted = true;

        if(GL_TRUE != glfwInit())
        {
            return 0;
        }

        //Use OpenGL 3.3 at least
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);

        if( !glfwOpenWindow(1920, 1080, 0, 0, 0, 0, 0, 0, GLFW_FULLSCREEN) )
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

        //theApp.getMapManager()->load("map/example.map");checkError();
       // theApp.getMapManager()->buildMap();checkError();
        //theApp.getMapManager()->saveMap();checkError();

        renderer->setDimensions((GLdouble)width, (GLdouble)height);

        SceneNodePtr cameraPtr(new FloatingCamera(&theApp, GAME_RENDER_GEOMETRY, 90.0f, 0.1f, 1000.0f));
        ((FloatingCamera*)cameraPtr.get())->yaw(3.1f);
        theApp.setEditor((FloatingCamera*)cameraPtr.get(), (FloatingCamera*)cameraPtr.get());

        sceneGraph->getRoot()->addChild(cameraPtr);

    /*
        Mesh* debugMesh = theApp.getResourceManager()->getMesh("debug_cube");
        SceneNodePtr meshPtr(new MeshNode(&theApp, GAME_RENDER_GEOMETRY, debugMesh));
        sceneGraph->addNode(meshPtr);

    */

        checkError();

        //End app setup
        glDisable(GL_DEPTH_TEST);checkError();
        glDisable(GL_CULL_FACE);checkError();

        //I basically messed up and had all my matrixes inverted, so all my geometry is wound the wrong way now...
        glfwDisable(GLFW_MOUSE_CURSOR);
        glFrontFace(GL_CCW);checkError();
        glCullFace(GL_BACK);checkError();
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);checkError();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);checkError();

        glfwSetWindowTitle("Game");checkError();

        checkError();

        std::vector<std::string> mapFiles;
        mapFiles.push_back("map/level1.map");
        mapFiles.push_back("map/level2.map");
        mapFiles.push_back("map/level3.map");

        int currentMap = 0;

        GLfloat windowCorners[] = { 0.0f, 0.0f,
                                    1.0f, 0.0f,
                                    1.0f, 1.0f,
                                    1.0f, 1.0f,
                                    0.0f, 1.0f,
                                    0.0f, 0.0f };


        VertexArrayObject* VAO = theApp.getResourceManager()->createVAO("splashscreen");
        ShaderProgram* renderImage = theApp.getResourceManager()->createProgramFromFiles("splash", "shader/render_image.vert", "shader/render_image.frag");
        Texture* splashScreen = theApp.getResourceManager()->createTextureFromFile("texture/splash.png");
        splashScreen->bind();
        splashScreen->generateMipMaps(10);
        splashScreen->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        splashScreen->setMaxFilter(GL_LINEAR);
        splashScreen->setWrapS(GL_REPEAT);
        splashScreen->setWrapT(GL_REPEAT);
        splashScreen->unbind();

        Buffer* windowCoords = theApp.getResourceManager()->createBuffer(GL_ARRAY_BUFFER, "splashscreen_quad");

        renderImage->use();             checkError();
        GLint positionLocation = renderImage->getAttribLocation("v2_position");

        VAO->init();
        VAO->bind();

        windowCoords->init();
        windowCoords->bind();

        windowCoords->setData(windowCorners, 12, sizeof(GLfloat));
        glEnableVertexAttribArray(positionLocation);             checkError();
        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

        windowCoords->unbind();



        int safeGuard = 0;

        while(notStarted || safeGuard > 10000)
        {
            //Display Splash Screen
            glClear(GL_COLOR_BUFFER_BIT);


            glActiveTexture(GL_TEXTURE0);             checkError();

            splashScreen->bind();             checkError();

            renderImage->setUniformMatrix4fv("m_MVP", glm::value_ptr(glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f)));             checkError();
                         checkError();

            glDrawArrays(GL_TRIANGLES,0,6);             checkError();
                     checkError();
            splashScreen->unbind();             checkError();

            glfwSwapBuffers();             checkError();
            notStarted = !theApp.playerHasStarted();             checkError();
            safeGuard++;
        }

        VAO->unbind();

        glEnable(GL_DEPTH_TEST);checkError();
        glEnable(GL_CULL_FACE);checkError();

        double fps = 0.0;
        unsigned int frames = 0;
        double lastFPSUpdate = 0.0;
        double elapsedTime = 0.0;

        while(running)
        {
            if(theApp.nextMap())
            {
                if(currentMap >= mapFiles.size())
                {
                    std::cout << "YOU WIN!" << std::endl;

                    glfwTerminate();

                    return 0;
                }
                else
                {
                    theApp.getMapManager()->load(mapFiles[currentMap]);
                    theApp.getMapManager()->buildMap();
                    theApp.getMapManager()->saveMap();
                    currentMap++;
                    theApp.useEditor();
                    SceneNodePtr crosshair(new Crosshair(&theApp, width/2, height/2, 10.0f, 1.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
                    sceneGraph->addNode(crosshair);
                }
            }

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
                fps = frames/(elapsedTime-lastFPSUpdate);
                lastFPSUpdate = elapsedTime;
                frames = 0;
            }
            //std::cout << "FPS: " << fps << std::endl;

            // exit if ESC was pressed or window was closed
            running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

            //End of level, reset scene graph.
            if(theApp.nextMap())
            {
                theApp.getSceneGraph()->setRoot(SceneNodePtr(new SceneNode(&theApp, GAME_RENDER_NONE)));
            }

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
