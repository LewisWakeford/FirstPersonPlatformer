#include "PlayerSpawn.h"
#include "PlayerSpawnNode.h"

PlayerSpawn::PlayerSpawn(App* app) : Persistant (app, "sp")
{
	addProperty("origin", new VectorF(3, 3), false);
}


PlayerSpawn::~PlayerSpawn(void)
{
}

bool PlayerSpawn::build()
{
	if(!getPropertyByName("origin")->getValue()->getVectorF(&mOrigin))
		return false;

	return true;
}

PersistantNode* PlayerSpawn::createNode()
{
    PlayerSpawnNode* node = new PlayerSpawnNode(mApp, mOrigin);
    mSceneNode = node;
    return node;
}

void PlayerSpawn::updateProperties()
{
    if(mSceneNode->hasChanged())
    {
        PlayerSpawnNode* node = (PlayerSpawnNode*)mSceneNode;
        mOrigin = node->getOrigin();
    }
}
