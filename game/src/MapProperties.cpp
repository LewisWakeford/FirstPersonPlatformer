#include "MapProperties.h"
#include "MapNode.h"


MapProperties::MapProperties(App* app) : Persistant(app, "H")
{
	addProperty("name", new String(), false);
	addProperty("display_name", new String(), true);
	addProperty("min_beacons", new Integer(), true);
}


MapProperties::~MapProperties(void)
{

}

bool MapProperties::build()
{
	bool valid = getPropertyByName("name")->getValue()->getString(&mMapName);
	valid = getPropertyByName("display_name")->getValue()->getString(&mDisplayName);
	valid = getPropertyByName("min_beacons")->getValue()->getInt(&mNumberOfBeacons);

	return valid;
}

PersistantNode* MapProperties::createNode()
{
    MapNode* node = new MapNode(mApp, mName, mDisplayName, mNumberOfBeacons);
    mSceneNode = node;
    return node;
}

void MapProperties::updateProperties()
{
    if(mSceneNode->hasChanged())
    {
        MapProperties* node = (MapProperties*)mSceneNode;
        mMapName = node->mMapName;
        mDisplayName = node->mDisplayName;
        mNumberOfBeacons = node->mNumberOfBeacons;
    }
}
