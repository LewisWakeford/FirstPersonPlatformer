#ifndef JUMPMAN_H
#define JUMPMAN_H

#include "I_Controllable.h"
#include "Collider.h"
#include "Orientation.h"
#include "RayShape.h"

class App;
class OrientationCamera;

class Jumpman : public I_Controllable, public SceneNode
{
    public:
        Jumpman(App* app, OrientationCamera* camera);
        virtual ~Jumpman();

        void simulateSelf(GLdouble deltaTime);

        void processInput(const ActionSet* actionSet);

        virtual void onCollision(CollisionEvent event);

        void setSpawn(float x, float y, float z);

        OrientationCamera* getCamera;

    protected:

        void applyGravity(double deltaTime);
        void applyAcceleration(double deltaTime);
        void limitVelocity(double deltaTime);

        glm::vec3 mAcceleration; //Current Acceleration.
        glm::vec3 mVelocity; //Current Velocity.

        int mForwardMovement;
        int mRightMovement;
        bool mCanMoveForward;
        bool mCanMoveBackward;
        bool mCanMoveLeft;
        bool mCanMoveRight;
        bool mCanJump;

        bool mJumping;
        bool mLeaping;
        bool mClimbing;

        glm::vec3 mClimbTarget;
        double mTimeAttemptingToClimb;
        static double sMaxTimeAttemptingToClimb;
        bool mAttemptingToClimb;
        bool mAttached;

        double mHeadHeight; //The distance from the origin that the camera is placed at.
        double mFeetHeight; //The distance from the origin that the feet collide with the world.

        bool mIsGrounded; //True if feet are on ground;

        Orientation mOrientation; //Does not change with camera, locked to always be in x/z plane.

        glm::vec3 mAim; //Where the player is aiming.

        double mAzimuth; //Absolute orientation in the x/z plain.
        double mAltitude; //Up/Down angle.

        double mDeltaAzi;
        double mDeltaAlt;

        double mDeltaTime; //Copy of this frames delta time, for rolling back movement after collisions.

        OrientationCamera* mFPCamera; //First person camera linked to this object.

        double mJumpTimerAll;
        double mJumpTimerGround;

        unsigned int mFeetNumber;
        unsigned int mFrontNumber;
        unsigned int mBackNumber;
        unsigned int mLeftNumber;
        unsigned int mRightNumber;
        unsigned int mTopNumber;
        unsigned int mRayNumber;

        RayShape* mCrosshairRay;

        static float sGrabRange;
        static float sLungeRange;
        static float sLungeVelocity;
        static float sFallLimit;
        static float sJumpAccel; //The amount of upwards acceleration added when jumping.
        static float sLeapAccel; //The amount of forwards acceleration added when leaping.

        static float sGroundMaxSpeed;
        static float sGroundAccel;
        static float sGroundDeaccel;
        static float sGroundFriction;

       static  float sAirMaxSpeed;
       static  float sAirAccelRatio;

        static float sMaxAltitude;
       static  float sMinAltitude;

       static  float sFixedLookSpeed;
       static  float sLookSpeedRatio;


        static float sJumpCooldownGround; //Time taken after landing to be able to jump again.
       static float sJumpCooldownAir; //Time after jumping before being able to jump again.

        bool mClimable; //There is a climbable wall under the crosshair.
        glm::vec3 mClimableCoord; //The world coordinate under the crosshair.

        glm::vec3 mSpawnPos;

    private:
};

#endif // JUMPMAN_H
