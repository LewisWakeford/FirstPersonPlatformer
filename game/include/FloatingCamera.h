#ifndef FLOATINGCAMERA_H
#define FLOATINGCAMERA_H

#include "OrientationCamera.h"
#include "I_Controllable.h"

/*
    Class: FloatingCamera
    A subclass of OrientationCamera that implements methods to help with controlling.
*/
class FloatingCamera : public OrientationCamera, public I_Controllable
{
    public:
        FloatingCamera(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar);
        virtual ~FloatingCamera();

        void simulateSelf(GLdouble deltaTime);

        void processInput(const ActionSet* actionSet);

    protected:

        void clearMovement();

        double mTransSpeed;
        double mRotSpeed;

        //Set on process input
        double mYawAmount;
        double mPitchAmount;
        double mRollAmount;
        double mWalkAmount;
        double mStrafeAmount;
        double mFlyAmount;


    private:
};

#endif // FLOATINGCAMERA_H
