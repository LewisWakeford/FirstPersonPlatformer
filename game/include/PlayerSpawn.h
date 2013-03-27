#pragma once
#include "Persistant.h"
class PlayerSpawn :
	public Persistant
{
public:
	PlayerSpawn(App* app);
	virtual ~PlayerSpawn(void);

	bool build();

	PersistantNode* createNode();

protected:

    void updateProperties();

	std::vector<float> mOrigin; //change to glm vector
};

