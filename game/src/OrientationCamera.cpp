#include "OrientationCamera.h"

#include "glm/gtc/matrix_inverse.hpp"
#include "App.h"
#include "Renderer.h"

OrientationCamera::OrientationCamera(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar)
    : CameraNode(app, renderPass, fov, zNear, zFar)
{
    //ctor
}

OrientationCamera::~OrientationCamera()
{
    //dtor
}

Orientation* OrientationCamera::orientation()
{
    return &mOrientation;
}

glm::mat4 OrientationCamera::getViewMatrix() const
{
    glm::mat4 temp = mApp->getRenderer()->currentMatrix();
    return temp;
}

glm::mat4 OrientationCamera::getTransform() const
{
    return mOrientation.getOrientationMatrix();
}

void OrientationCamera::lookAt(SceneNode* sceneNode)
{
    mTarget = sceneNode;
}

void OrientationCamera::pitch(GLfloat angle)
{
    mOrientation.pitch(angle);
}

void OrientationCamera::yaw(GLfloat angle)
{
    mOrientation.yaw(angle);
}

void OrientationCamera::roll(GLfloat angle)
{
    mOrientation.roll(angle);
}

void OrientationCamera::fly(GLfloat amount)
{
    mOrientation.fly(amount);
}

void OrientationCamera::strafe(GLfloat amount)
{
    mOrientation.strafe(amount);
}

void OrientationCamera::walk(GLfloat amount)
{
    mOrientation.walk(amount);
}
