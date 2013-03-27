#include "CuboidShape.h"

CuboidShape::CuboidShape(glm::vec3 pos, float xSize, float ySize, float zSize)
{
    mPos = pos;
    mXSize = xSize;
    mYSize = ySize;
    mZSize = zSize;
    mType = SHAPE_CUBOID;
}

CuboidShape::CuboidShape(float x, float y, float z, float xSize, float ySize, float zSize)
{
    mPos = glm::vec3(x, y, z);
    mXSize = xSize;
    mYSize = ySize;
    mZSize = zSize;
    mType = SHAPE_CUBOID;
}

CuboidShape::CuboidShape()
{
    mPos = glm::vec3(0.0f, 0.0f, 0.0f);
    mXSize = 0.0f;
    mYSize = 0.0f;
    mZSize = 0.0f;
    mType = SHAPE_CUBOID;
}

CuboidShape::~CuboidShape()
{
    //dtor
}

float CuboidShape::getXSize()
{
    return mXSize;
}

float CuboidShape::getYSize()
{
    return mYSize;
}

float CuboidShape::getZSize()
{
    return mZSize;
}

glm::vec3 CuboidShape::getPos()
{
    return mPos;
}

BoundingBox CuboidShape::getBoundingBox() const
{
    return BoundingBox(mPos.x, mPos.y, mPos.z, mXSize, mYSize, mZSize);
}
