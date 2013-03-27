#include "Renderer.h"
#include "Mesh.h"
#include "CameraNode.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::pushMatrix(glm::mat4 matrix)
{
    if(mMatrixStack.size() > 0)
    {
        glm::mat4 temp = mMatrixStack.back();
        temp = temp * matrix;
        mMatrixStack.push_back(temp);
    }
    else
    {
        glm::mat4 temp = matrix;
        mMatrixStack.push_back(temp);
    }
}

glm::mat4 Renderer::popMatrix()
{
    glm::mat4 temp = mMatrixStack.back();
    mMatrixStack.pop_back();
    return temp;
}

glm::mat4 Renderer::currentMatrix()
{
    glm::mat4 temp = mMatrixStack.back();
    return temp;
}

glm::mat4 Renderer::getViewMatrix()
{
    return mViewMatrix;
}

glm::mat4 Renderer::getProjectionMatrix()
{
    return mProjectionMatrix;
}

GLdouble Renderer::getDisplayHeight()
{
    return mViewportHeight;
}

GLdouble Renderer::getDisplayWidth()
{
    return mViewportWidth;
}

void Renderer::setDimensions(GLdouble width, GLdouble height)
{
    mViewportWidth = width;
    mViewportHeight = height;
}

void Renderer::setCamera(CameraNode* camera)
{
    mCurrentCamera =camera;
    mProjectionMatrix = camera->getProjectionMatrix();
}

void Renderer::updateCamera(CameraNode* camera)
{
    if(mCurrentCamera == camera)
    {
        glm::mat4 temp = glm::inverse(camera->getViewMatrix());
        mViewMatrix = temp;
    }
}


