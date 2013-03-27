#include "WayPoint.h"
#include "WayPointNode.h"


WayPoint::WayPoint(App* app) : Persistant (app, "wp")
{
	addProperty("origin", new VectorF(3, 3), false);
}


WayPoint::~WayPoint(void)
{
}

bool WayPoint::build()
{
	if(!getPropertyByName("origin")->getValue()->getVectorF(&mOrigin))
		return false;

	return true;
}

PersistantNode* WayPoint::createNode()
{
    WayPointNode* node = new WayPointNode(mApp, mOrigin);
    mSceneNode = node;
    return node;
}

void WayPoint::updateProperties()
{
    if(mSceneNode->hasChanged())
    {
        WayPointNode* node = (WayPointNode*)mSceneNode;
        mOrigin = node->getOrigin();
    }
}
