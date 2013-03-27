#ifndef ORIENTATIONCAMERA_H
#define ORIENTATIONCAMERA_H

#include "CameraNode.h"
#include "Orientation.h"

/*
    Class: OrientationCamera
    A CameraNode that uses an Orientation object to make it easy to control.
    The pitch, roll, yaw, strafe, walk and fly methods call the underlying orientation object.
*/

class OrientationCamera : public CameraNode
{
    public:
        //Nothing special here, simply calls CameraNode's constructor.
        OrientationCamera(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar);
        virtual ~OrientationCamera();

        //Reorient the camera to face a specific scene node.
        //TODO: Implement the tracking feature.
        void lookAt(SceneNode* sceneNode);

        //Should probably remove this.
        Orientation* orientation();

        //Returns the transform of the orientation object instead of this node's transform.
        virtual glm::mat4 getViewMatrix() const;
        glm::mat4 getTransform() const;

        //These functions call the identical functions on the orientation class:
        void pitch(GLfloat angle);
        void yaw(GLfloat angle);
        void roll(GLfloat angle);

        void fly(GLfloat amount);
        void strafe(GLfloat amount);
        void walk(GLfloat amount);

    protected:
        //The orientation object.
        Orientation mOrientation;

        //The target of the camera tracking.
        SceneNode* mTarget;

    private:
};

#endif // ORIENTATIONCAMERA_H
