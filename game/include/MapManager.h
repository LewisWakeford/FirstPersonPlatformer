#pragma once

#include <vector>

#include "Persistant.h"

class App;

class MapManager
{
public:
	MapManager(App* app);
	~MapManager(void);

	void reset();
	bool load(std::string filename);

	//Use shared pointer.
	void buildMap();
	bool saveMap();

protected:

	bool mInCommentBlock;
	bool mInPersistant;
	std::string mCurrentPersistantName;
	unsigned int mCurrentPersistantIndex;
	std::string mMapString;
	std::vector<Persistant*> mPersistantList;

	bool addPersistantByTag(std::string tagName);
	void clearPersistants();

	bool mReadyForBuild;

    App* mApp;

};

