#include "Crosshair.h"

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "App.h"
#include "ResourceManager.h"
#include "Renderer.h"

Crosshair::Crosshair(App* app, GLfloat xPos, GLfloat yPos, GLfloat scale, GLfloat thickness, glm::vec4 color) : SceneNode(app, GAME_RENDER_UI_2D)
{
    mInitilaized = false;
    mXPos = xPos;
    mYPos = yPos;
    mThickness = thickness;
    mScale = scale;
    mColor = color;
}

Crosshair::~Crosshair()
{
    //dtor
}

void Crosshair::renderSelf()
{
    mProgram->use();
    mProgram->setUniform1f("f_xOffset", mXPos);
    mProgram->setUniform1f("f_yOffset", mYPos);
    mProgram->setUniform4f("v4_color", glm::value_ptr(mColor));
    mProgram->setUniformMatrix4fv("m_MVP", glm::value_ptr(glm::ortho(0.0f, float(mApp->getRenderer()->getDisplayWidth()), float(mApp->getRenderer()->getDisplayHeight()), 0.0f, -1.0f, 1.0f)));

    mVAO->bind();
    mIndexBuffer->bind();

    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
}

void Crosshair::simulateSelf(double deltaTime)
{
    if(!mInitilaized)
    {
        init();
        mInitilaized = true;
    }
}

void Crosshair::init()
{
    //Create vertex arrays:
    GLfloat* vertexArray = new GLfloat[24]; //12 2d vertices
    GLuint* indexArray = new GLuint[30];

    vertexArray[0] = -mScale; vertexArray[1] = mThickness;
    vertexArray[2] = -mThickness; vertexArray[3] = mThickness;
    vertexArray[4] = -mThickness; vertexArray[5] = mScale;
    vertexArray[6] = mThickness; vertexArray[7] = mScale;
    vertexArray[8] = mThickness; vertexArray[9] = mThickness;
    vertexArray[10] = mScale; vertexArray[11] = mThickness;
    vertexArray[12] = mScale; vertexArray[13] = -mThickness;
    vertexArray[14] = mThickness; vertexArray[15] = -mThickness;
    vertexArray[16] = mThickness; vertexArray[17] = -mScale;
    vertexArray[18] = -mThickness; vertexArray[19] = -mScale;
    vertexArray[20] = -mThickness; vertexArray[21] = -mThickness;
    vertexArray[22] = -mScale; vertexArray[23] = -mThickness;

    indexArray[0] = 1; indexArray[1] = 10; indexArray[2] = 0;
    indexArray[3] = 10; indexArray[4] = 11; indexArray[5] = 0;
    indexArray[6] = 2; indexArray[7] = 4; indexArray[8] = 1;
    indexArray[9] = 3; indexArray[10] = 4; indexArray[11] = 2;
    indexArray[12] = 6; indexArray[13] = 7; indexArray[14] = 4;
    indexArray[15] = 5; indexArray[16] = 6; indexArray[17] = 4;
    indexArray[18] = 8; indexArray[19] = 9; indexArray[20] = 10;
    indexArray[21] = 7; indexArray[22] = 8; indexArray[23] = 10;
    indexArray[24] = 7; indexArray[25] = 10; indexArray[26] = 1;
    indexArray[27] = 4; indexArray[28] = 7; indexArray[29] = 1;

    //GL initialisation:

    mProgram = mApp->getResourceManager()->getProgram("generic_ui");

    GLuint posistionLocation = mProgram->getAttribLocation("v2_position");

    ArrayBuffer* vertexBuffer = mApp->getResourceManager()->createArrayBuffer(GL_ARRAY_BUFFER, "crosshair");
    vertexBuffer->init();
    vertexBuffer->bind();
    vertexBuffer->setData(vertexArray, 24, sizeof(GLfloat));
    vertexBuffer->unbind();

    GLuint vertexArrayName = 0;
    vertexBuffer->setArray(vertexArrayName, 2, GL_FLOAT, 0, 0, posistionLocation);

    mVAO = mApp->getResourceManager()->createVAO("crosshair");
    mVAO->init();
    mVAO->bind();
    mVAO->linkVertexArray(posistionLocation, vertexBuffer, vertexArrayName);
    mVAO->unbind();

    mIndexBuffer = mApp->getResourceManager()->createArrayBuffer(GL_ELEMENT_ARRAY_BUFFER, "crosshair_index");
    mIndexBuffer->init();
    mIndexBuffer->bind();
    mIndexBuffer->setData(indexArray, 30, sizeof(GLfloat));
    mIndexBuffer->unbind();

    delete[] vertexArray;
    delete[] indexArray;
}
