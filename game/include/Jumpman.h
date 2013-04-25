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

        void teleport(float x, float y, float z);

        OrientationCamera* getCamera;

    protected:

        void applyGravity(double deltaTime);
        void applyAcceleration(double deltaTime);
        void limitVelocity(double deltaTime);

        glm::vec3 mAcceleration; //Current Acceleration.
        glm::vec3 mVelocity; //Current Velocity.

        //All measures in metres, seconds and newtons.
        double mGroundMaxSpeed; //The maximum speed you can build up to when grounded, before friction starts to slow you.
        double mGroundAccel; //The acceleration rate while your feet are grounded.
        double mGroundDeaccel; //The deacceleration rate when you try to move in an opposing direction to the current one.
        double mGroundFriction; //Percentage of speed lost per second when over the ground speed limit;

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

        double mJumpCooldown; //Time in seconds after landing you must wait before attempting to jump again.
        double mJumpTimer;    //While non-zero, cannot jump. Decrements every tick.

        double mAirMaxSpeed; //Terminal Velocity
        double mAirAccelRatio; //How much percentage of the regular acceleration can you perform in mid-air. 1.0 gives almost perfect air control, 0.0 is realistic.

        float mJumpAccel; //The amount of upwards acceleration added when jumping.
        float mLeapAccel; //The amount of forwards acceleration added when leaping.

        double mHeadHeight; //The distance from the origin that the camera is placed at.
        double mFeetHeight; //The distance from the origin that the feet collide with the world.

        bool mIsGrounded; //True if feet are on ground;

        Orientation mOrientation; //Does not change with camera, locked to always be in x/z plane.

        glm::vec3 mAim; //Where the player is aiming.

        double mAzimuth; //Absolute orientation in the x/z plain.
        double mAltitude; //Up/Down angle.
        double mMaxAltitude; //How far "up" the player can look. At 1/2 pi they cannot look further back than directly up.
        double mMinAltitude; //How far "down" the player can look. At 1/2 pi they cannot look further forward than directly down.

        double mDeltaAzi;
        double mDeltaAlt;
        double mFixedLookSpeed; //The angle per second that you look around when using a digital input like keys.
        double mLookSpeedRatio; //The angle per second per unit of movment on an analogue input. Basically, mouse sensitivity.

        double mDeltaTime; //Copy of this frames delta time, for rolling back movement after collisions.

        OrientationCamera* mFPCamera; //First person camera linked to this object.

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

        bool mClimable; //There is a climbable wall under the crosshair.
        glm::vec3 mClimableCoord; //The world coordinate under the crosshair.

    private:
};

#endif // JUMPMAN_H
