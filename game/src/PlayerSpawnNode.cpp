#include "PlayerSpawnNode.h"
#include "App.h"
#include "SceneGraph.h"
#include "OrientationCamera.h"
#include "Jumpman.h"

PlayerSpawnNode::PlayerSpawnNode(App* app, std::vector<float> origin) : PersistantNode(app, GAME_RENDER_NONE)
{
    mOrigin = origin;
    mSpawnedPlayer = false;
}

PlayerSpawnNode::~PlayerSpawnNode()
{
    //dtor
}

std::vector<float> PlayerSpawnNode::getOrigin()
{
    return mOrigin;
}

void PlayerSpawnNode::simulateSelf(double deltaTime)
{
    if(!mSpawnedPlayer) spawnPlayer();
}

void PlayerSpawnNode::spawnPlayer()
{
    OrientationCamera* FPCamera = new OrientationCamera(mApp, GAME_RENDER_GEOMETRY, 90.0f, 0.1f, 1000.0f);
    SceneNodePtr jumpmanPtr(new Jumpman(mApp, FPCamera));
    ((Jumpman*)jumpmanPtr.get())->teleport(mOrigin[0], mOrigin[1], mOrigin[2]);
    mApp->setRegular((Jumpman*)jumpmanPtr.get(), FPCamera);
    mApp->useRegular();
    mApp->getSceneGraph()->getRoot()->addChild(jumpmanPtr);

    mSpawnedPlayer = true;
}
