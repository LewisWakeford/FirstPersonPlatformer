#ifndef RAYSHAPE_H
#define RAYSHAPE_H

#include "CollisionShape.h"

#include <glm/glm.hpp>

class RayShape : public CollisionShape
{
    public:
        RayShape(glm::vec3 origin, glm::vec3 direction);
        virtual ~RayShape();

        BoundingBox getBoundingBox() const;

        const glm::vec3& getOrigin() const;
        const glm::vec3& getDirection() const;

        void setDirection(const glm::vec3& direction);

    protected:

        glm::vec3 mOrigin; //Offset from parent object.
        glm::vec3 mDirection; //Destination Relative to parent.

    private:
};

#endif // RAYSHAPE_H
