#pragma once
#include "Persistant.h"

enum BLOCK_FACE
{
    BLOCK_FACE_NORTH,
    BLOCK_FACE_EAST,
    BLOCK_FACE_SOUTH,
    BLOCK_FACE_WEST,
    BLOCK_FACE_UP,
    BLOCK_FACE_DOWN
};

class Block : public Persistant
{
public:
	Block(App* app);
	virtual ~Block(void);

	bool build();
	PersistantNode* createNode();



protected:

    void updateProperties();

	std::vector<float> mOrigin; //Change to glm vector.

	int mNegX;
	int mPosX;
	int mNegY;
	int mPosY;
	int mNegZ;
	int mPosZ;

	std::vector<int> mNorthFace;
	std::vector<int> mEastFace;
	std::vector<int> mSouthFace;
	std::vector<int> mWestFace;

	float mRotation;
};

