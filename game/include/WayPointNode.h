#ifndef WAYPOINTNODE_H
#define WAYPOINTNODE_H

#include <PersistantNode.h>

class Mesh;

class WayPointNode : public PersistantNode
{
    public:
        WayPointNode(App* app, std::vector<float> origin);
        virtual ~WayPointNode();

        std::vector<float> getOrigin();

        void simulateSelf(GLdouble deltaTime);
        void renderSelf();

        void onCollision(CollisionEvent event);

    protected:
        std::vector<float> mOrigin; //change to glm vector

        Mesh* mMesh;

    private:
};

#endif // WAYPOINTNODE_H
