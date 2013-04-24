#ifndef collisionSYSTEM_H
#define collisionSYSTEM_H

#include "Collider.h"

class CollisionSystem
{
    public:
        CollisionSystem();
        virtual ~CollisionSystem();

        void pushCollider(Collider* collider);

        void checkCollisions();

        void pushMatrix(glm::mat4 matrix);
        glm::mat4 popMatrix();
        glm::mat4 currentMatrix();

        bool broadphaseTest(const Collider* A, const Collider* B);
        bool narrowphaseTest(const Collider* A, const Collider* B, std::vector<glm::vec3>* contactPoints, double* time);

        bool narrowCuboidOnCuboid(const Collider* A, const Collider* B, std::vector<glm::vec3>* contactPoints, double* time);

        //Casts a ray out into the world
        bool narrowRayOnCuboid(const Collider* rayCollider, const Collider* CubeoidCollider, std::vector<glm::vec3>* contactPoints, double* time);

        //Returns false if no intersection, point of interesction returned in result.
        bool lineIntersection(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2* result);

        //Returns true if the lines intersect at a point with the two points that define each line.
        bool lineIntersectionInBounds(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2* result);

    protected:
        std::vector<Collider*> mObjectList; //The list of nodes we need to check for collisions next simulation.

        std::vector<glm::mat4> mMatrixStack;

    private:
};

#endif // collisionSYSTEM_H
