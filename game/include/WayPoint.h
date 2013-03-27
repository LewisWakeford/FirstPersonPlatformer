#pragma once
#include "Persistant.h"
class WayPoint :
	public Persistant
{
public:
	WayPoint(App* app);
	virtual ~WayPoint(void);

	bool build();

	PersistantNode* createNode();

protected:

    void updateProperties();

	std::vector<float> mOrigin; //change to glm vector
};

