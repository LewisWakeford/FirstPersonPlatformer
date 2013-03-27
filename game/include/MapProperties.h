#pragma once
#include "Persistant.h"
class MapProperties :
	public Persistant
{
public:
	MapProperties(App* app);
	virtual ~MapProperties(void);

	bool build();

	PersistantNode* createNode();

protected:


    void updateProperties();

	std::string mMapName;
	std::string mDisplayName;
	int mNumberOfBeacons;
};

