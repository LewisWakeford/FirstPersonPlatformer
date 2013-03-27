#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include <glm/glm.hpp>

#include "CollisionShape.h"


class CuboidShape : public CollisionShape
{
    public:
        CuboidShape();
        CuboidShape(glm::vec3 pos, float xSize, float ySize, float zSize);
        CuboidShape(float x, float y, float z, float xSize, float ySize, float zSize);
        virtual ~CuboidShape();

        float getXSize();
        float getYSize();
        float getZSize();

        glm::vec3 getPos();

        virtual BoundingBox getBoundingBox() const;

    protected:

        //Size in each dimension.
        float mXSize;
        float mYSize;
        float mZSize;

        //Origin
        glm::vec3 mPos;

    private:
};

#endif // CUBESHAPE_H
