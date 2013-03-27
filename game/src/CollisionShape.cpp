#include "CollisionShape.h"

CollisionShape::CollisionShape()
{
    mType = SHAPE_INVALID;
}

CollisionShape::~CollisionShape()
{
    //dtor
}

const CollisionShapeType CollisionShape::getType() const
{
    return mType;
}

BoundingBox CollisionShape::getBoundingBox() const
{
    //Super class returns no bounding box.
    return BoundingBox(0,0,0,0,0,0);
}

BoundingBox BoundingBox::composite(const BoundingBox& A, const BoundingBox& B)
{
    float minX = A.mX;
    float minY = A.mY;
    float minZ = A.mZ;
    float maxX = A.mX + A.mXSize;
    float maxY = A.mY + A.mYSize;
    float maxZ = A.mZ + A.mZSize;

    if(minX > B.mX) minX = B.mX;
    if(minY > B.mY) minY = B.mY;
    if(minZ > B.mZ) minZ = B.mZ;
    if(maxX < B.mX + B.mXSize) maxX = B.mXSize;
    if(maxY < B.mY + B.mYSize) maxY = B.mYSize;
    if(maxZ < B.mZ + B.mZSize) maxZ = B.mZSize;

    BoundingBox comp(minX, minY, minZ, maxX-minX, maxY-minY, maxZ-minZ);
    return comp;
}

bool BoundingBox::overlap(const BoundingBox& A, const BoundingBox& B)
{
    //Check for overlap in X
    bool xOverlap = false;
    if(A.mX >= B.mX && A.mX <= B.mX + B.mXSize) xOverlap = true;
    else if(A.mX + A.mXSize >= B.mX && A.mX + A.mXSize <= B.mX + B.mXSize) xOverlap = true;
    else if(B.mX >= A.mX && B.mX <= A.mX + A.mXSize) xOverlap = true;
    else if(B.mX + B.mXSize >= A.mX && B.mX + B.mXSize <= A.mX + A.mXSize) xOverlap = true;

    //Check for overlap in Y
    bool yOverlap = false;
    if(A.mY >= B.mY && A.mY <= B.mY + B.mYSize) yOverlap = true;
    else if(A.mY + A.mYSize >= B.mY && A.mY + A.mYSize <= B.mY + B.mYSize) yOverlap = true;
    else if(B.mY >= A.mY && B.mY <= A.mY + A.mYSize) yOverlap = true;
    else if(B.mY + B.mYSize >= A.mY && B.mY + B.mYSize <= A.mY + A.mYSize) yOverlap = true;

    //Check for overlap in Z
    bool zOverlap = false;
    if(A.mZ >= B.mZ && A.mZ <= B.mZ + B.mZSize) zOverlap = true;
    else if(A.mZ + A.mZSize >= B.mZ && A.mZ + A.mZSize <= B.mZ + B.mZSize) zOverlap = true;
    else if(B.mZ >= A.mZ && B.mZ <= A.mZ + A.mZSize) zOverlap = true;
    else if(B.mZ + B.mZSize >= A.mZ && B.mZ + B.mZSize <= A.mZ + A.mZSize) zOverlap = true;

    if(xOverlap && yOverlap && zOverlap)
    {
        return true;
    }
    else
    {
        return false;
    }
}
