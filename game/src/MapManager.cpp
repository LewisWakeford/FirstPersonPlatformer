#include <iostream>
#include <fstream>

#include "MapManager.h"
#include "App.h"
#include "MapProperties.h"
#include "Block.h"
#include "PlayerSpawn.h"
#include "WayPoint.h"
#include "SceneGraph.h"



MapManager::MapManager(App* app)
{
	mReadyForBuild = false;
	mInCommentBlock = false;
	mInPersistant = false;
	mCurrentPersistantIndex = 0;
	mApp = app;
}


MapManager::~MapManager(void)
{
    clearPersistants();
}

void MapManager::reset()
{
	mReadyForBuild = false;
	mInCommentBlock = false;
	mInPersistant = false;
	mCurrentPersistantIndex = 0;
	clearPersistants();
}

void MapManager::clearPersistants()
{
    for(size_t i = 0; i < mPersistantList.size(); i++)
    {
       delete mPersistantList[i];
    }
    mPersistantList.clear();
}

bool MapManager::addPersistantByTag(std::string tagName)
{
	if(tagName == "H")
	{
		//Add header object
		mPersistantList.push_back(new MapProperties(mApp));
		return true;
	}
	else if(tagName == "bl")
	{
		//Add block
		mPersistantList.push_back(new Block(mApp));
		return true;
	}
	else if(tagName == "sp")
	{
		//Add player spawn point
		mPersistantList.push_back(new PlayerSpawn(mApp));
		return true;
	}
	else if(tagName == "wp")
	{
		//Add waypoint/beacon
		mPersistantList.push_back(new WayPoint(mApp));
		return true;
	}
	else
	{
		return false;
	}
}

bool MapManager::load(std::string filename)
{
	mPersistantList.clear();
	std::ifstream file;
	file.open(filename.c_str(), std::ios_base::in);

	if(!file.is_open())
	{
		//ERROR
		return false;
	}

	std::string currentChunk;

	while(file.good())
	{
		char currentChar = file.get();

		if(mInCommentBlock)
		{
			 //Within comment block so ignore all chars except the end comment char: '#'
			if(currentChar == '#')
			{
				mInCommentBlock = false;
			}
		}
		else if(mInPersistant)
		{
			if(currentChar == '#')
			{
				mInCommentBlock = true;
			}
			else
			{
				if(currentChar == '<') //Found new tag
				{
					//Find rest of tag.
					std::string tagName;
					std::string currentTag;

					currentTag += currentChar;

					currentChar = file.get();
					currentTag += currentChar;

					while(currentChar != '>')
					{
						tagName += currentChar;
						currentChar = file.get();
						currentTag += currentChar;
					}

					bool closing = (tagName.substr(0, 1) == "/");

					if(closing) tagName.erase(0, 1);

					//If this is the end of this persistant object, try to parse the current chunk.
					if(closing && tagName == mCurrentPersistantName)
					{
						//Parse the chunk then reset.
						if(!mPersistantList[mCurrentPersistantIndex]->parse(currentChunk))
							return false;
						currentChunk = "";
						mInPersistant = false;
					}
					else
					{
						currentChunk += currentTag; //If this is not the closing tag it should be passed to the persistant.
					}
				}
				else
				{
					currentChunk += currentChar; //Add char to current chunk.
				}
			}
		}
		else
		{
			if(currentChar == '#')
			{
				mInCommentBlock = true;
			}
			else if(currentChar == '<')
			{
				//Find rest of tag.
				std::string tagName;
				std::string currentTag;

				currentTag += currentChar;

				currentChar = file.get();
				currentTag += currentChar;

				while(currentChar != '>')
				{
					tagName += currentChar;
					currentChar = file.get();
					currentTag += currentChar;
				}

				bool closing = (tagName.substr(0, 1) == "/");

				if(closing)
				{
					//error
					return false;
				}

				if(addPersistantByTag(tagName))
				{
					mCurrentPersistantIndex = mPersistantList.size()-1;
					mCurrentPersistantName = tagName;
					mInPersistant = true;
				}
				else
				{
					//error
					return false;
				}
			}
		}
	}

	file.close();
	return true;
}

void MapManager::buildMap()
{
    for(size_t i = 0; i < mPersistantList.size(); i++)
    {
        mPersistantList[i]->build();
        SceneNodePtr node = SceneNodePtr(mPersistantList[i]->createNode());
        mApp->getSceneGraph()->addNode(node);
    }
    mApp->mapLoaded();
}

bool MapManager::saveMap()
{
    std::string output = "";
    for(size_t i = 0; i < mPersistantList.size(); i++)
    {
        output += mPersistantList[i]->toString();
    }
    //dump string to file.
    std::cout << "MAP STRING: " << std::endl << std::endl << output;
    return false;
}
