#include "Block.h"

#include "BlockNode.h"


Block::Block(App* app) : Persistant(app, "bl")
{
	addProperty("origin", new VectorF(3, 3), false);
	addProperty("n_x", new Integer(), true);
	addProperty("p_x", new Integer(), true);
	addProperty("n_y", new Integer(), true);
	addProperty("p_y", new Integer(), true);
	addProperty("n_z", new Integer(), true);
	addProperty("p_z", new Integer(), true);
	addProperty("north", new VectorI(2), true);
	addProperty("east", new VectorI(2), true);
	addProperty("south", new VectorI(2), true);
	addProperty("west", new VectorI(2), true);
	addProperty("rot", new Float(), true);
}


Block::~Block(void)
{

}

bool Block::build()
{
	if(!getPropertyByName("origin")->getValue()->getVectorF(&mOrigin))
		return false;

	if(!getPropertyByName("n_x")->getValue()->getInt(&mNegX))
		mNegX = 0;

	if(!getPropertyByName("p_x")->getValue()->getInt(&mPosX))
		mPosX = 0;

	if(!getPropertyByName("n_y")->getValue()->getInt(&mNegY))
		mNegY = 0;

	if(!getPropertyByName("p_y")->getValue()->getInt(&mPosY))
		mPosY = 0;

	if(!getPropertyByName("n_z")->getValue()->getInt(&mNegZ))
		mNegZ = 0;

	if(!getPropertyByName("p_z")->getValue()->getInt(&mPosZ))
		mPosZ = 0;

	getPropertyByName("north")->getValue()->getVectorI(&mNorthFace);
	getPropertyByName("east")->getValue()->getVectorI(&mEastFace);
	getPropertyByName("south")->getValue()->getVectorI(&mSouthFace);
	getPropertyByName("west")->getValue()->getVectorI(&mWestFace);

	if(!getPropertyByName("rot")->getValue()->getFloat(&mRotation))
		mRotation = 0.0f;

	return true;
}

PersistantNode* Block::createNode()
{
    BlockNode* node = new BlockNode(mApp);

    node->setOrigin(mOrigin);
    node->setXSize(mNegX, mPosX);
    node->setYSize(mNegY, mPosY);
    node->setZSize(mNegZ, mPosZ);
    node->setNorthFace(mNorthFace);
    node->setEastFace(mEastFace);
    node->setSouthFace(mSouthFace);
    node->setWestFace(mWestFace);
    node->setRotation(mRotation);

    mSceneNode = node;

    return node;
}

void Block::updateProperties()
{
    if(mSceneNode->hasChanged())
    {
        BlockNode* node = (BlockNode*)mSceneNode;
        mOrigin = node->getOrigin();
        mNegX = node->getNegX();
        mPosX = node->getPosX();
        mNegY = node->getNegY();
        mPosY = node->getPosY();
        mNegZ = node->getNegZ();
        mPosZ = node->getPosZ();

        mNorthFace = node->getNorthFace();
        mEastFace = node->getEastFace();
        mSouthFace = node->getSouthFace();
        mWestFace = node->getWestFace();

        mRotation = node->getRotation();
    }
}
