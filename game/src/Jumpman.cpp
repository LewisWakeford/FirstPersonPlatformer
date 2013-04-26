#include "Jumpman.h"

#include "OrientationCamera.h"
#include "CuboidShape.h"
#include "RayShape.h"
#include "App.h"
#include "BlockNode.h"

#include "glm/core/type_vec3.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

float Jumpman::sLungeRange = 2.0f;
float Jumpman::sGrabRange = 1.5f;
float Jumpman::sLungeVelocity = 10.0f;
double Jumpman::sMaxTimeAttemptingToClimb = 1.0;

Jumpman::Jumpman(App* app, OrientationCamera* camera) : SceneNode(app, GAME_RENDER_NONE)
{
    mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    mVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

    mIsGrounded = false;

    mCanMoveForward = true;
    mCanMoveBackward = true;
    mCanMoveLeft = true;
    mCanMoveRight = true;
    mCanJump = true;


    mForwardMovement = 0;
    mRightMovement = 0;
    mJumping = false;
    mLeaping = false;
    mClimbing = false;

    mClimbTarget = glm::vec3(0.0f);
    mTimeAttemptingToClimb = 0.0;
    mAttemptingToClimb = false;
    mAttached = false;

    mGroundMaxSpeed = 15.0f;
    mGroundAccel = 8.0f;
    mGroundDeaccel = 30.0f;
    mGroundFriction = 20.0f;

    mAirMaxSpeed = 200.0f;
    mAirAccelRatio = 0.5f;

    mJumpAccel = 5.0f;
    mLeapAccel = 10.0f;

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

    mClimable = false;
    mClimableCoord = glm::vec3(0.0f);

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
    mRayNumber = 6;

    CuboidShape* feet = new CuboidShape(-0.15f, -mFeetHeight, -0.15f, 0.3f, 0.3f, 0.3f);
    CuboidShape* back = new CuboidShape(-0.25f, -mFeetHeight+0.25f, 0.15, 0.5f, mHeadHeight+mFeetHeight, 0.25f);
    CuboidShape* front = new CuboidShape(-0.25f, -mFeetHeight+0.25f, -0.35, 0.5f, mHeadHeight+mFeetHeight, 0.25f);
    CuboidShape* left = new CuboidShape(-0.35f, -mFeetHeight+0.25f, -0.25f, 0.25f, mHeadHeight+mFeetHeight, 0.5f);
    CuboidShape* right = new CuboidShape(0.15f, -mFeetHeight+0.25f, -0.25f, 0.25f, mHeadHeight+mFeetHeight, 0.5f);
    CuboidShape* top = new CuboidShape(-0.25f, mHeadHeight, -0.25f, 0.3f, 0.3f, 0.53f);
    mCrosshairRay = new RayShape(glm::vec3(0.0f, mHeadHeight, 0.0f), glm::vec3(0.0f, 0.0f, -sLungeRange)); //Keep a reference for when we move the camera.

    addCollider(new Collider(feet, mFeetNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(front, mFrontNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(back, mBackNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(left, mLeftNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(right, mRightNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(top, mTopNumber, this, GAME_COLLISION_PLAYER, GAME_COLLISION_BLOCK));
    addCollider(new Collider(mCrosshairRay, mRayNumber, this, GAME_COLLISION_NONE, GAME_COLLISION_BLOCK));
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
            mVelocity.y = 0; //Should totally cancel vertical component.
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

    if(speed < 0.005f)
    {
        mVelocity = glm::vec3(0.0f);
    }

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
    mClimbing = false;
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

    if(actionSet->getB(ACTION_B_CLIMB))
    {
        mClimbing = true;
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

    float bounce = 1.3f;
    float bounceMin = 0.2f;

    unsigned int reference = collider->getRefNumber();

    if(reference == mFeetNumber)
    {
        //std::cout << " FEET " << std::endl;
        mIsGrounded = true;

         std::vector<glm::vec3> contactPoints = event.getContactPoints();
         float yOverlap = contactPoints[0].y;

         if(yOverlap > 0)
         {
             mOrientation.translate(0.0f, yOverlap, 0.0f);
         }

        //glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getUp());
        //mOrientation.translate(rollback.x, rollback.y, rollback.z);
    }
    else if(reference == mRayNumber)
    {
        std::vector<glm::vec3> contactPoints = event.getContactPoints();

        //Should only be one point, but just make sure.
        for(int i = 0; i < contactPoints.size(); i++)
        {
            BlockNode* blockNode = (BlockNode*) event.collidedWith()->getMaster();
            mClimable = blockNode->isClimable(contactPoints[i]);
            mClimableCoord = contactPoints[i];
            //std::cout << " RAYHIT: "; if(climbable) std::cout << "brick" << std::endl; else std::cout << "metal" << std::endl;
            //std::cout << "     X: " << contactPoints[i].x << " Y: " << contactPoints[i].y <<" Z:" << contactPoints[i].z << std::endl;
        }
    }
    else
    {
        if(reference == mFrontNumber)
        {
            mCanMoveForward = false;
            glm::vec3 inverseForward = -mOrientation.getForward();
            float forwardVelocity = glm::dot(inverseForward, mVelocity);

            if(forwardVelocity > 0.005f)
            {
                //glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getForward());
                //mOrientation.translate(rollback.x, rollback.y, rollback.z);
                mVelocity -= ( ((bounce * forwardVelocity)+bounceMin) * inverseForward);
                std::cout << "FRONT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
            }
        }
        else if(reference == mBackNumber)
        {
            mCanMoveBackward = false;
            glm::vec3 inverseForward = -mOrientation.getForward();
            float forwardVelocity = glm::dot(inverseForward, mVelocity);

            if(forwardVelocity < 0.005f)
            {
                //glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getForward());
                //mOrientation.translate(rollback.x, rollback.y, rollback.z);
                mVelocity -= (((bounce * forwardVelocity)-bounceMin) * inverseForward);
                std::cout << "BACK: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
            }
        }
        else if(reference == mLeftNumber)
        {
            mCanMoveLeft = false;
            float rightVelocity = glm::dot(mOrientation.getRight(), mVelocity);

            if(rightVelocity < 0.005f)
            {
                //glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getRight());
                //mOrientation.translate(rollback.x, rollback.y, rollback.z);
                mVelocity -= (((bounce * rightVelocity)-bounceMin) * mOrientation.getRight());
                std::cout << "LEFT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
            }
        }
        else if(reference == mRightNumber)
        {
            mCanMoveRight = false;
            float rightVelocity = glm::dot(mOrientation.getRight(), mVelocity);

            if(rightVelocity > 0.005f)
            {
                //glm::vec3 rollback = float(rollbackTime) * (-mVelocity * mOrientation.getRight());
                //mOrientation.translate(rollback.x, rollback.y, rollback.z);
                mVelocity -= (((bounce * rightVelocity)+bounceMin) * mOrientation.getRight());
                std::cout << "RIGHT: " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << std::endl;
            }
        }
        else if(reference == mTopNumber)
        {
            mCanJump = false;
            float upVelocity = glm::dot(mOrientation.getUp(), mVelocity);

            if(upVelocity > 0.005f)
            {
                mVelocity -= (((bounce * upVelocity)-bounceMin) * mOrientation.getUp());
            }
            //std::cout << " TOP " << std::endl;
        }
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
    if(mForwardMovement == 1)
    {
        if(forwardVelocity > mGroundMaxSpeed || !mCanMoveForward)
            canMoveForward = false;
    }
    else if(mForwardMovement == -1)
    {
        if(forwardVelocity < -mGroundMaxSpeed || !mCanMoveBackward)
            canMoveForward = false;
    }

    bool canMoveRight = true;
    if(mRightMovement == 1)
    {
        if(rightVelocity > mGroundMaxSpeed || !mCanMoveRight)
            canMoveRight = false;
    }
    else if(mRightMovement == -1 )
    {
        if(rightVelocity < -mGroundMaxSpeed || !mCanMoveLeft)
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

    if((mIsGrounded || mAttached) && mCanJump)
    {
        if(mJumpTimer > 0.0 && upVelocity <= 0.0f)
        {
            mJumpTimer -= deltaTime;
            if(mJumpTimer < 0.0) mJumpTimer = 0.0;
        }

        if(mJumping && mJumpTimer <= 0.0)
        {
            mVelocity += mJumpAccel * mOrientation.getUp() ;
            mJumpTimer = mJumpCooldown;
            mAttached = false;
        }
        else if(mLeaping && mJumpTimer <= 0.0)
        {
            mVelocity -= mLeapAccel * mAim;
            mJumpTimer = mJumpCooldown;
            mAttached = false;
        }
    }

    //Climbing Code

    glm::vec3 myWorldPos = mOrientation.getPos();
    if(mClimbing)
    {
        //If not already attached to something, attempt to climb.
        if(mClimable && !mAttached && !mAttemptingToClimb)
        {
            glm::vec3 vectorToClimbPoint = mClimableCoord - myWorldPos;
            float distanceToClimbPoint = glm::length(vectorToClimbPoint);

            if(distanceToClimbPoint <= sLungeRange)
            {
                //Lunge towards the climbpoint if out of range.
                if(distanceToClimbPoint > sGrabRange)
                {
                    mVelocity += glm::normalize(vectorToClimbPoint) * sLungeVelocity;

                }

                std::cout << "Attempting to Climb" << std::endl;
                mAttemptingToClimb = true;
                mTimeAttemptingToClimb = 0.0;

                mClimbTarget = mClimableCoord;
            }


        }
        //Detach if attached.
        else if(mAttached)
        {
            mAttached = false;
        }
    }

    if(mAttemptingToClimb && !mClimbing)
    {
        glm::vec3 vectorToClimbPoint = mClimbTarget - myWorldPos;
        float distanceToClimbPoint = glm::length(vectorToClimbPoint);
        std::cout << distanceToClimbPoint << std::endl;
        if(distanceToClimbPoint <= sGrabRange)
        {
            mVelocity = glm::vec3(0.0f); //Kill all movement
            mAcceleration = glm::vec3(0.0f);

            mAttached = true;
            mAttemptingToClimb = false;
        }
        else
        {
            mTimeAttemptingToClimb += deltaTime;
        }
    }

    if(mTimeAttemptingToClimb >= sMaxTimeAttemptingToClimb)
    {
        mAttemptingToClimb = false;
        std::cout << "Give up" << std::endl;
    }

    mAcceleration = (mOrientation.getForward() * -forwardAccel) + (mOrientation.getRight() * rightAccel);

    if(!mAttached)
    {
        applyAcceleration(deltaTime);
        applyGravity(deltaTime);
        limitVelocity(deltaTime);
    }

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

    glm::vec3 forward = mFPCamera->orientation()->getForward();
    forward.x = -(forward.x);
    forward.z = -(forward.z);
    mCrosshairRay->setDirection(mFPCamera->orientation()->getForward() * -sLungeRange);
    //mCrosshairRay->setDirection(forward * sGrabRange);

    mTransform = mOrientation.getOrientationMatrix();

    mIsGrounded = false;
    mCanMoveForward = true;
    mCanMoveBackward = true;
    mCanMoveLeft = true;
    mCanMoveRight = true;
    mCanJump = true;

}

void Jumpman::teleport(float x, float y, float z)
{
    glm::vec3 teleportPos(x,y,z);
    mOrientation.setPos(teleportPos);
}
