#include "RayShape.h"

RayShape::RayShape(glm::vec3 origin, glm::vec3 direction)
{
    mOrigin = origin;
    mDirection = direction;
    mType = SHAPE_RAY;
}

RayShape::~RayShape()
{
    //dtor
}

BoundingBox RayShape::getBoundingBox() const
{
    float x;
    float y;
    float z;
    float xSize;
    float ySize;
    float zSize;

    if(mDirection.x > 0)
    {
        x = mOrigin.x;
        xSize = mDirection.x;
    }
    else if (mDirection.x < 0)
    {
        // X <----- O
        //
        x = mOrigin.x + mDirection.x;
        xSize = (- mDirection.x);
    }
    else
    {
        x = mOrigin.x-0.1f;
        xSize  = 0.2f;
    }

    if(mDirection.y > 0)
    {
        y = mOrigin.y;
        ySize = mDirection.y;
    }
    else if(mDirection.y < 0)
    {
        y = mOrigin.y + mDirection.y;
        ySize = (- mDirection.y);
    }
    else
    {
        y = mOrigin.y-0.1f;
        ySize = 0.2f;
    }

    if(mDirection.z > 0)
    {
        z = mOrigin.z;
        zSize = mDirection.z;
    }
    else if (mDirection.z < 0)
    {
        z = mOrigin.z + mDirection.z;
        zSize = (- mDirection.z);
    }
    else
    {
        z = mOrigin.z - 0.1f;
        zSize = 0.2f;
    }

    return BoundingBox(x,y,z, xSize, ySize, zSize);
}

const glm::vec3& RayShape::getOrigin() const
{
    return mOrigin;
}

const glm::vec3& RayShape::getDirection() const
{
    return mDirection;
}

void RayShape::setDirection(const glm::vec3& direction)
{
    mDirection = direction;
}
