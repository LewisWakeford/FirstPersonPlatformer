#ifndef COLLIDER_H
#define COLLIDER_H

#include "game.h"

#include "SceneNode.h"
#include "CollisionShape.h"
#include "Orientation.h"

#include <glm/glm.hpp>

#include <vector>

//Any scene node can have collision nodes as a component of them.
//collision nodes will send messages back to their "parent".

class CollisionSystem;
class CollisionEvent;

class Collider
{
    public:

        //Master gives the collider a reference number that is returned with any collision events.
        Collider(CollisionShape* shape, const unsigned int referenceNumber, I_CollisionReciever* master, GLenum group, GLenum collidesWith);
        virtual ~Collider();

        void setGroup(GLuint group);
        void setCollidesWith(GLuint group);

        Orientation getOrientation(double timeStepFraction) const;   //Get the collision shape at that fraction of the time step.
                                                            //0.0 returns the previous shape at the end of the last step.
                                                            //1.0 returns the current shape at the end of this time step.

        CollisionShape* getShape() const;
        BoundingBox getBoundingBox(double timeStepFraction) const;

        void transformBy(glm::mat4 transform); //Give the collider a new transform. The old transform is kept so that they can be interpolated between.

        GLuint getGroup();
        GLboolean collidesWith(GLuint group);

        void addEvent(CollisionEvent event);
        void processCollisions();

        unsigned int getRefNumber();

    protected:

        bool mInitialized;

        GLenum mCollisionGroup;
        GLenum mCollidesWith;   //The groups that this object colldies with. If the object hits something it does not collide with it will be unaffected.

        std::vector<CollisionEvent> mCollisionEvents;

        I_CollisionReciever* mMaster;
        unsigned int mReference;

        CollisionShape* mCollisionShape;

        Orientation mPrevTransform;
        Orientation mCurrentTransform;
        Orientation mOriginalTransform;

    private:
};

#endif // COLLIDER_H
