#ifndef PLAYERSPAWNNODE_H
#define PLAYERSPAWNNODE_H

#include <PersistantNode.h>


class PlayerSpawnNode : public PersistantNode
{
    public:
        PlayerSpawnNode(App* app, std::vector<float> origin);
        virtual ~PlayerSpawnNode();

        std::vector<float> getOrigin();

    protected:
        std::vector<float> mOrigin; //change to glm vector
    private:
};

#endif // PLAYERSPAWNNODE_H
