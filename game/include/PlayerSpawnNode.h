#ifndef PLAYERSPAWNNODE_H
#define PLAYERSPAWNNODE_H

#include <PersistantNode.h>


class PlayerSpawnNode : public PersistantNode
{
    public:
        PlayerSpawnNode(App* app, std::vector<float> origin);
        virtual ~PlayerSpawnNode();

        std::vector<float> getOrigin();

        virtual void simulateSelf(double deltaTime);

    protected:
        void spawnPlayer();

        std::vector<float> mOrigin; //change to glm vector

        bool mSpawnedPlayer;
    private:
};

#endif // PLAYERSPAWNNODE_H
