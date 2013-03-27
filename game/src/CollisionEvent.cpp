#include "CollisionEvent.h"

CollisionEvent::CollisionEvent(Collider* collider, Collider* collidedWith, std::vector<glm::vec3> contactPoints, double time)
{
    mCollider = collider;
    mCollidedWith = collider;
    mContactPoints = contactPoints;
    mCollisionTime = time;
}

CollisionEvent::~CollisionEvent()
{
    //dtor
}

Collider* CollisionEvent::collidedWith()
{
    return mCollidedWith;
}

Collider* CollisionEvent::myCollider()
{
    return mCollider;
}

double CollisionEvent::getCollisionTime()
{
    return mCollisionTime;
}
