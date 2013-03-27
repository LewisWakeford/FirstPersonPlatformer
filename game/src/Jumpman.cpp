#include "Jumpman.h"

#include "OrientationCamera.h"
#include "CuboidShape.h"
#include "App.h"

#include "glm/core/type_vec3.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

Jumpman::Jumpman(App* app, OrientationCamera* camera) : SceneNode(app, GAME_RENDER_NONE)
{
    mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    mVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

    mIsGrounded = false;

    mForwardMovement = 0;
    mRightMovement = 0;
    mJumping = false;
    mLeaping = false;

    mGroundMaxSpeed = 15.0f;
    mGroundAccel = 8.0f;
    mGroundDeaccel = 30.0f;
    mGroundFriction = 20.0f;

    mAirMaxSpeed = 200.0f;
    mAirAccelRatio = 0.5f;

    mJumpAccel = 5.0f;
    mLeapAccel = 5.0f;

    mAim = mOrientation.getForward();

    mAzimuth = 0.0f;
    mAltitude = 0.0f;
    mMaxAltitude = 90.0f;
    mMinAltitude = -90.0f;

    mHeadHeight = 0.6f;
    mFeetHeight = 0.8f;

    mJumpTimer = 0.0;
    mJumpCooldown = 0.05;

    mDeltaAzi = 0.0;
    mDeltaAlt = 0.0;
    mFixedLookSpeed = 60.0;
    mLookSpeedRatio = 180.0;

    SceneNodePtr cameraPtr(camera);
    addChild(cameraPtr);

    mFPCamera = camera;
    camera->orientation()->reset();
    camera->fly(mHeadHeight);

    mFeetNumber = 0;
    mFrontNumber = 1;
    mBackNumber = 2;
    mLeftNumber = 3;
    mRightNumber = 4;
    mTopNumber = 5;

    CuboidShape* feet = new CuboidShape(-0.25f, -mFeetHeight, -0.25f, 0.5f, 0.5f, 0.5f);
    CuboidShape* back = new CuboidShape(-0.25f, -mFeetHeight+0.5f, 0.25, 0.5f, 1.3f, 0.25f);
    CuboidShape* front = new CuboidShape(-0.25f, -mFeetHeight+0.5f, -0.5, 0.5f, 1.3f, 0.25f);
    CuboidShape* left = new CuboidShape(-0.5f, -mFeetHeight+0.5f, -0.25f, 0.25f, 1.3f, 0.5f);
    CuboidShape* right = new CuboidShape(0.25f, -mFeetHeight+0.5f, -0.25f, 0.25f, 1.3f, 0.5f);
    CuboidShape* top = new CuboidShape(-0.25f, 1.3f, -0.25f, 0.5f, 0.5f, 0.5f);

    addCollider(new Collider(feet, mFeetNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(front, mFrontNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(back, mBackNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(left, mLeftNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(right, mRightNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(top, mTopNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
}

Jumpman::~Jumpman()
{
    //dtor
}

void Jumpman::applyGravity(double deltaTime)
{
    if(!mIsGrounded)
    {
        float gravity = -(mApp->gGravity) * deltaTime;
        glm::vec3 gravityVector = mOrientation.getUp() * gravity;
        mVelocity += gravityVector;
    }
    else
    {
        float verticalVelocity = glm::dot(mVelocity, mOrientation.getUp());
        if(verticalVelocity < 0) //Moving down relative to up vector.
        {
            mVelocity -= verticalVelocity * mOrientation.getUp(); //Should totally cancel vertical component.
        }
    }
}

void Jumpman::applyAcceleration(double deltaTime)
{
    //Valid acceleration vector should have already been calculated else where.
    if(mIsGrounded)
    {
        mVelocity += mAcceleration * float(deltaTime);
    }
    else
    {
        mVelocity += mAcceleration * float(mAirAccelRatio * deltaTime);
    }
}

void Jumpman::limitVelocity(double deltaTime)
{
    float speed = glm::length(mVelocity);

    //Speed cannot be greater than max air speed.
    if(speed > mAirMaxSpeed)
    {
        float slowDownRatio = mAirMaxSpeed/speed;
        mVelocity *= slowDownRatio;
    }

    float verticalVelocity = glm::dot(mVelocity, mOrientation.getUp());

    //If grounded we need to apply friction.
    if(mIsGrounded && speed > mGroundMaxSpeed && verticalVelocity < 0)
    {
        float slowDownRatio = mGroundMaxSpeed/speed;
        float frictionRatio = 1.0 - (mGroundFriction * deltaTime);
        if(slowDownRatio < frictionRatio)
        {
            mVelocity *= slowDownRatio;
        }
        else
        {
            mVelocity *= frictionRatio;
        }
    }
}

void Jumpman::processInput(const ActionSet* actionSet)
{
    mForwardMovement = 0;
    mRightMovement = 0;
    mJumping = false;
    mLeaping = false;
    mDeltaAzi = 0.0;
    mDeltaAlt = 0.0;

    if(actionSet->getB(ACTION_B_MV_FW) && !actionSet->getB(ACTION_B_MV_BW))
    {
        mForwardMovement = 1;
    }
    if(!actionSet->getB(ACTION_B_MV_FW) && actionSet->getB(ACTION_B_MV_BW))
    {
        mForwardMovement = -1;
    }

    if(actionSet->getB(ACTION_B_MV_RI) && !actionSet->getB(ACTION_B_MV_LF))
    {
        mRightMovement = 1;
    }
    if(!actionSet->getB(ACTION_B_MV_RI) && actionSet->getB(ACTION_B_MV_LF))
    {
        mRightMovement = -1;
    }

    if(actionSet->getB(ACTION_B_JUMP))
    {
        mJumping = true;
    }

    if(actionSet->getB(ACTION_B_LEAP))
    {
        mLeaping = true;
    }

    if(actionSet->getB(ACTION_B_PITCH_P) && !actionSet->getB(ACTION_B_PITCH_N))
    {
        mDeltaAlt = mFixedLookSpeed;
    }
    if(!actionSet->getB(ACTION_B_PITCH_P) && actionSet->getB(ACTION_B_PITCH_N))
    {
        mDeltaAlt = -mFixedLookSpeed;
    }

    if(actionSet->getB(ACTION_B_YAW_P) && !actionSet->getB(ACTION_B_YAW_N))
    {
        mDeltaAzi = mFixedLookSpeed;
    }
    if(!actionSet->getB(ACTION_B_YAW_P) && actionSet->getB(ACTION_B_YAW_N))
    {
        mDeltaAzi = -mFixedLookSpeed;
    }

    mDeltaAlt += mLookSpeedRatio * actionSet->getD(ACTION_D_AIM_V);
    mDeltaAzi += mLookSpeedRatio * actionSet->getD(ACTION_D_AIM_H);

}

void Jumpman::onCollision(CollisionEvent event)
{
    Collider* collider = event.myCollider();

    double rollbackTime = event.getCollisionTime() * mDeltaTime;

    unsigned int reference = collider->getRefNumber();

    if(reference == mFeetNumber)
    {
        std::cout << " FEET " << std::endl;
        mIsGrounded = true;

        glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getUp());
        mOrientation.translate(rollback.x, rollback.y, rollback.z);
    }
    else if(reference == mFrontNumber)
    {
        glm::vec3 inverseForward = -mOrientation.getForward();
        float forwardVelocity = glm::dot(inverseForward, mVelocity);

        if(forwardVelocity > 0)
        {
            glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getForward());
            mOrientation.translate(rollback.x, rollback.y, rollback.z);
            mVelocity -= (forwardVelocity * inverseForward);
            std::cout << "FRONT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
        }
    }
    else if(reference == mBackNumber)
    {
        glm::vec3 inverseForward = -mOrientation.getForward();
        float forwardVelocity = glm::dot(inverseForward, mVelocity);

        if(forwardVelocity < 0)
        {
            glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getForward());
            mOrientation.translate(rollback.x, rollback.y, rollback.z);
            mVelocity -= (forwardVelocity * inverseForward);
            std::cout << "BACK: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
        }
    }
    else if(reference == mLeftNumber)
    {
        float rightVelocity = glm::dot(mOrientation.getRight(), mVelocity);

        if(rightVelocity < 0)
        {
            glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getRight());
            mOrientation.translate(rollback.x, rollback.y, rollback.z);
            mVelocity -= (rightVelocity * mOrientation.getRight());
            std::cout << "LEFT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
        }
    }
    else if(reference == mRightNumber)
    {
        float rightVelocity = glm::dot(mOrientation.getRight(), mVelocity);

        if(rightVelocity > 0)
        {
            glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getRight());
            mOrientation.translate(rollback.x, rollback.y, rollback.z);
            mVelocity -= (rightVelocity * mOrientation.getRight());
            std::cout << "RIGHT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
        }
    }
    else if(reference == mTopNumber)
    {
        std::cout << " TOP " << std::endl;
    }
}

void Jumpman::simulateSelf(double deltaTime)
{
    mDeltaTime = deltaTime;

    //Build acceleration vector.
    mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    //Get speed velocity components
    float forwardVelocity = glm::dot(mVelocity, mOrientation.getForward());
    float rightVelocity = glm::dot(mVelocity, mOrientation.getRight());
    float upVelocity = glm::dot(mVelocity, mOrientation.getUp());

    float forwardAccel = 0.0;
    float rightAccel = 0.0;
    float upAccel = 0.0;
    float aimAccel = 0.0;

    //If the direction we want to move is the opposite of the direction we are currently moving, then acceleration is increased.
    int currentForwardMovement = 0;
    int currentRightMovement = 0;

    //Don't bother accelerating if we are already going too fast.
    bool canMoveForward = true;
    if(forwardVelocity > mGroundMaxSpeed && mForwardMovement == 1)
    {
        canMoveForward = false;
    }
    else if(forwardVelocity < -mGroundMaxSpeed && mForwardMovement == -1)
    {
        canMoveForward = false;
    }

    bool canMoveRight = true;
    if(rightVelocity > mGroundMaxSpeed && mRightMovement == 1)
    {
        canMoveRight = false;
    }
    else if(rightVelocity < -mGroundMaxSpeed && mRightMovement == -1)
    {
        canMoveRight = false;
    }

    if(forwardVelocity < 0) currentForwardMovement = 1;
    if(forwardVelocity > 0) currentForwardMovement = -1;

    if(mForwardMovement != 0 && canMoveForward)
    {
        if(mForwardMovement != 0 && currentForwardMovement != 0 && mForwardMovement != currentForwardMovement)
        {
            forwardAccel = mGroundDeaccel;
        }
        else
        {
            forwardAccel = mGroundAccel;
        }
        forwardAccel *= mForwardMovement;
    }
    else if(mForwardMovement == 0)
    {
        forwardAccel = -(currentForwardMovement) * mGroundDeaccel;
    }

    if(rightVelocity > 0) currentRightMovement = 1;
    if(rightVelocity < 0) currentRightMovement = -1;

    if(mRightMovement != 0 && canMoveRight)
    {
        if(mRightMovement != 0 && currentRightMovement != 0 && mRightMovement != currentRightMovement) rightAccel = mGroundDeaccel;
        else  rightAccel = mGroundAccel;
        rightAccel *= mRightMovement;
    }
    else if(mRightMovement == 0)
    {
        rightAccel = (-currentRightMovement) * mGroundDeaccel;
    }

    if(mIsGrounded)
    {
        if(mJumpTimer > 0.0)
        {
            mJumpTimer -= deltaTime;
            if(mJumpTimer < 0.0) mJumpTimer = 0.0;
        }

        if(mJumping && mJumpTimer <= 0.0)
        {
            mVelocity += mJumpAccel * mOrientation.getUp() ;
            mJumpTimer = mJumpCooldown;
        }
        else if(mLeaping && mJumpTimer <= 0.0)
        {
            mVelocity -= mLeapAccel * mAim;
            mJumpTimer = mJumpCooldown;
        }
    }

    mAcceleration = (mOrientation.getForward() * -forwardAccel) + (mOrientation.getRight() * rightAccel);

    applyAcceleration(deltaTime);
    applyGravity(deltaTime);
    limitVelocity(deltaTime);

    mOrientation.translate(mVelocity.x*deltaTime, mVelocity.y*deltaTime, mVelocity.z*deltaTime);

    //Re-orient camera and aim vector.
    mAzimuth += mDeltaAzi * deltaTime;
    mAltitude += mDeltaAlt * deltaTime;
    if(mAltitude > mMaxAltitude) mAltitude = mMaxAltitude;
    if(mAltitude < mMinAltitude) mAltitude = mMinAltitude;

    //Only change forward and right vectors.
    mOrientation.resetRotation();
    mOrientation.yaw(mAzimuth);

    //Never roll.
    mFPCamera->orientation()->resetRotation();
    mFPCamera->orientation()->pitch(mAltitude);

    //Aim vector is the same as camera forward.
    mAim = glm::rotate(mOrientation.getForward(), float(mAltitude), mOrientation.getRight());

    mTransform = mOrientation.getOrientationMatrix();

    mIsGrounded = false;

}
