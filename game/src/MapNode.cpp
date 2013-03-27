#include "MapNode.h"

MapNode::MapNode(App* app, std::string name, std::string displayName, int numBeacons) : PersistantNode(app, GAME_RENDER_NONE)
{
    mMapName = name;
    mDisplayName = displayName;
    mNumberOfBeacons = numBeacons;
}

MapNode::~MapNode()
{
    //dtor
}
