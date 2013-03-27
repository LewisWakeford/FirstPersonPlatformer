#include "FloatingCamera.h"

#include "ActionSet.h"

FloatingCamera::FloatingCamera(App* app, GLenum renderPass, GLdouble fov, GLdouble zNear, GLdouble zFar)
    : OrientationCamera(app, renderPass, fov, zNear, zFar)
{
    mTransSpeed = 2.0f;
    mRotSpeed = 50.0f;
    clearMovement();
}

FloatingCamera::~FloatingCamera()
{

}

void FloatingCamera::simulateSelf(GLdouble deltaTime)
{
    bool matrixChanged = false;

    if(mPitchAmount != 0.0) {pitch(mPitchAmount*deltaTime); matrixChanged = true;}
    if(mYawAmount != 0.0) {yaw(mYawAmount*deltaTime); matrixChanged = true;}
    if(mRollAmount != 0.0) {roll(mRollAmount*deltaTime); matrixChanged = true;}

    if(mWalkAmount != 0.0) {walk(mWalkAmount*deltaTime); matrixChanged = true;}
    if(mStrafeAmount != 0.0) {strafe(mStrafeAmount*deltaTime); matrixChanged = true;}
    if(mFlyAmount != 0.0) {fly(mFlyAmount*deltaTime); matrixChanged = true;}

    //TODO: Maybe Optimize the recaluation of the matrix
}

void FloatingCamera::processInput(const ActionSet* actionSet)
{
    clearMovement();
    //Translate
    if(actionSet->getB(ACTION_B_MV_FW) && !actionSet->getB(ACTION_B_MV_BW))
    {
        mWalkAmount = mTransSpeed;
    }
    if(actionSet->getB(ACTION_B_MV_BW) && !actionSet->getB(ACTION_B_MV_FW))
    {
        mWalkAmount = -mTransSpeed;
    }

    if(actionSet->getB(ACTION_B_MV_LF) && !actionSet->getB(ACTION_B_MV_RI))
    {
        mStrafeAmount = -mTransSpeed;
    }
    if(actionSet->getB(ACTION_B_MV_RI) && !actionSet->getB(ACTION_B_MV_LF))
    {
        mStrafeAmount = mTransSpeed;
    }

    if(actionSet->getB(ACTION_B_MV_UP) && !actionSet->getB(ACTION_B_MV_DW))
    {
        mFlyAmount = mTransSpeed;
    }
    if(actionSet->getB(ACTION_B_MV_DW) && !actionSet->getB(ACTION_B_MV_UP))
    {
        mFlyAmount = -mTransSpeed;
    }

    //Rotate
    if(actionSet->getB(ACTION_B_PITCH_P) && !actionSet->getB(ACTION_B_PITCH_N))
    {
        mPitchAmount = -mRotSpeed;
    }
    if(actionSet->getB(ACTION_B_PITCH_N) && !actionSet->getB(ACTION_B_PITCH_P))
    {
        mPitchAmount = mRotSpeed;
    }

    if(actionSet->getB(ACTION_B_YAW_P) && !actionSet->getB(ACTION_B_YAW_N))
    {
        mYawAmount = -mRotSpeed;
    }
    if(actionSet->getB(ACTION_B_YAW_N) && !actionSet->getB(ACTION_B_YAW_P))
    {
        mYawAmount = mRotSpeed;
    }

    if(actionSet->getB(ACTION_B_ROLL_P) && !actionSet->getB(ACTION_B_ROLL_N))
    {
        mRollAmount = -mRotSpeed;
    }
    if(actionSet->getB(ACTION_B_ROLL_N) && !actionSet->getB(ACTION_B_ROLL_P))
    {
        mRollAmount = mRotSpeed;
    }
}

void FloatingCamera::clearMovement()
{
    mYawAmount = 0.0;
    mPitchAmount = 0.0;
    mRollAmount = 0.0;
    mWalkAmount = 0.0;
    mStrafeAmount = 0.0;
    mFlyAmount = 0.0;
}
