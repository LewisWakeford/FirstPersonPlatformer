#include "App.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "CollisionEvent.h"

Collider::Collider(CollisionShape* shape, const unsigned int referenceNumber, I_CollisionReciever* master, GLenum collisionGroup, GLenum collidesWithGroup)
{
    mCollisionShape = shape;
    mReference = referenceNumber;
    mMaster = master;
    mInitialized = false;
    mCollisionGroup = collisionGroup;
    mCollidesWith = collidesWithGroup;
}

Collider::~Collider()
{
    if(mCollisionShape != 0)
    {
        delete mCollisionShape;
    }
}

I_CollisionReciever* Collider::getMaster()
{
    return mMaster;
}

GLuint Collider::getGroup()
{
    return mCollisionGroup;
}

GLboolean Collider::collidesWith(GLuint group)
{
    return group & mCollidesWith;
}

void Collider::transformBy(glm::mat4 transform)
{
    if(mInitialized)
    {
        mPrevTransform = mCurrentTransform;
        mCurrentTransform = mOriginalTransform;
        mCurrentTransform.transform(transform);
    }
    else
    {
        mCurrentTransform.transform(transform);
        mPrevTransform = mCurrentTransform;
        mInitialized = true;
    }

}

unsigned int Collider::getRefNumber()
{
    return mReference;
}

Orientation Collider::getOrientation(double timeStepFraction) const
{
    return mPrevTransform;
    //return Orientation::mix(mPrevTransform, mCurrentTransform, timeStepFraction);
}

CollisionShape* Collider::getShape() const
{
    return mCollisionShape;
}

void Collider::addEvent(CollisionEvent event)
{
    mCollisionEvents.push_back(event);
}

void Collider::processCollisions()
{
    for(unsigned int i = 0; i < mCollisionEvents.size(); i++)
    {
        mMaster->onCollision(mCollisionEvents[i]);
    }
    mCollisionEvents.clear();
}

void Collider::setGroup(GLuint group)
{
    mCollisionGroup = group;
}

void Collider::setCollidesWith(GLuint group)
{
    mCollidesWith = group;
}

BoundingBox Collider::getBoundingBox(double timeStepFraction) const
{
    BoundingBox untransformedBox = mCollisionShape->getBoundingBox();
    Orientation orientation = getOrientation(timeStepFraction);

    std::vector<glm::vec3> points;

    points.push_back(glm::vec3(untransformedBox.mX                             , untransformedBox.mY                           , untransformedBox.mZ));
    points.push_back(glm::vec3(untransformedBox.mX + untransformedBox.mXSize   , untransformedBox.mY                           , untransformedBox.mZ));
    points.push_back(glm::vec3(untransformedBox.mX                             , untransformedBox.mY + untransformedBox.mYSize , untransformedBox.mZ));
    points.push_back(glm::vec3(untransformedBox.mX                             , untransformedBox.mY                           , untransformedBox.mZ + untransformedBox.mZSize));
    points.push_back(glm::vec3(untransformedBox.mX + untransformedBox.mXSize   , untransformedBox.mY + untransformedBox.mYSize , untransformedBox.mZ));
    points.push_back(glm::vec3(untransformedBox.mX                             , untransformedBox.mY + untransformedBox.mYSize , untransformedBox.mZ + untransformedBox.mZSize));
    points.push_back(glm::vec3(untransformedBox.mX + untransformedBox.mXSize   , untransformedBox.mY                           , untransformedBox.mZ + untransformedBox.mZSize));
    points.push_back(glm::vec3(untransformedBox.mX + untransformedBox.mXSize   , untransformedBox.mY + untransformedBox.mYSize , untransformedBox.mZ + untransformedBox.mZSize));

    glm::mat4 transform = orientation.getOrientationMatrix();

    for(unsigned int i = 0; i < points.size(); i++)
    {
        points[i] = glm::vec3(transform * glm::vec4(points[i], 1.0f));
    }

    float minX = points[0].x;
    float maxX = points[0].x;
    float minY = points[0].y;
    float maxY = points[0].y;
    float minZ = points[0].z;
    float maxZ = points[0].z;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        if(points[i].x < minX) minX = points[i].x;
        if(points[i].y < minY) minY = points[i].y;
        if(points[i].z < minZ) minZ = points[i].z;
        if(points[i].x > maxX) maxX = points[i].x;
        if(points[i].y > maxY) maxY = points[i].y;
        if(points[i].z > maxZ) maxZ = points[i].z;
    }

    BoundingBox transformedBox(minX, minY, minZ, maxX-minX, maxY-minY, maxZ-minZ);

    return transformedBox;
}
