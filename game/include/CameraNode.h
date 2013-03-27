#ifndef CAMERANODE_H
#define CAMERANODE_H

#include "SceneNode.h"

class CameraNode : public SceneNode
{
    public:
        CameraNode(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar);
        virtual ~CameraNode();

        void renderSelf();

        GLdouble getFOV();
        GLdouble getZNear();
        GLdouble getZFar();

        virtual glm::mat4 getProjectionMatrix() const;
        virtual glm::mat4 getViewMatrix() const;

    protected:
        GLdouble mFOV;
        GLdouble mZNear;
        GLdouble mZFar;

        glm::mat4 mProjectionMatrix;

    private:
};

#endif // CAMERANODE_H
