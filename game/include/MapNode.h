#ifndef MAPNODE_H
#define MAPNODE_H

#include <PersistantNode.h>


class MapNode : public PersistantNode
{
    public:
        MapNode(App* app, std::string name, std::string displayName, int numBeacons);
        virtual ~MapNode();

    protected:
        std::string mMapName;
        std::string mDisplayName;
        int mNumberOfBeacons;

    private:
};

#endif // MAPNODE_H
