#ifndef RAYSHAPE_H
#define RAYSHAPE_H

#include <CollisionShape.h>

//TODO
/*
    Ray detection.
    We need to be able to retrieve the type of tile (climable/climable) under the crosshair.
    So we need to get the collider we hit, plus the relative contact point on that collider.
    Also would be a good idea to build a isClimable() function for the block class that can take vec3s.
*/

class RayShape : public CollisionShape
{
    public:
        RayShape(glm::vec3 origin, glm::vec3 direction, float length);
        virtual ~RayShape();

        BoundingBox getBoundingBox() const;

    protected:

        glm::vec3 mOrigin;
        glm::vec3 mDirection;
        float mRange;


    private:
};

#endif // RAYSHAPE_H
