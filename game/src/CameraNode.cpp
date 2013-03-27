#include "CameraNode.h"
#include "App.h"
#include "Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

CameraNode::CameraNode(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar) :
    SceneNode(app, renderPass)
{
    mFOV = fov;
    mZNear = zNear;
    mZFar = zFar;

    mProjectionMatrix = glm::perspective(fov, mApp->getRenderer()->getDisplayWidth()/mApp->getRenderer()->getDisplayHeight(), zNear, zFar);
}

CameraNode::~CameraNode()
{
    //dtor
}

void CameraNode::renderSelf()
{
   mApp->getRenderer()->updateCamera(this);
}

GLdouble CameraNode::getFOV()
{
    return mFOV;
}

GLdouble CameraNode::getZNear()
{
    return mZNear;
}

GLdouble CameraNode::getZFar()
{
    return mZFar;
}

glm::mat4 CameraNode::getViewMatrix() const
{
    return getTransform();
}

glm::mat4 CameraNode::getProjectionMatrix() const
{
    return mProjectionMatrix;
}
