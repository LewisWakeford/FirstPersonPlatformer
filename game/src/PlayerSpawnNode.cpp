#include "PlayerSpawnNode.h"

PlayerSpawnNode::PlayerSpawnNode(App* app, std::vector<float> origin) : PersistantNode(app, GAME_RENDER_NONE)
{
    mOrigin = origin;
}

PlayerSpawnNode::~PlayerSpawnNode()
{
    //dtor
}

std::vector<float> PlayerSpawnNode::getOrigin()
{
    return mOrigin;
}
