#ifndef collisionEVENT_H
#define collisionEVENT_H

#include <vector>

#include "glm/glm.hpp"

class Collider;

class CollisionEvent
{
    public:
        CollisionEvent(Collider* collider, Collider* collidedWith, std::vector<glm::vec3> contactPoints, double time);
        virtual ~CollisionEvent();

        Collider* collidedWith();
        Collider* myCollider();
        const std::vector<glm::vec3>& getContactPoints();
        double getCollisionTime();

    protected:
        Collider* mCollidedWith;    //The collider that was collided with.
        Collider* mCollider;        //The collider owned by the object that will recieve this message.

        std::vector<glm::vec3> mContactPoints;    //The point at which collision occured.
        double mCollisionTime;      //How many seconds after the start of the tick did collision occur?

    private:
};

#endif // collisionEVENT_H
