#include "PersistantNode.h"

PersistantNode::PersistantNode(App* app, GLenum renderPass) : SceneNode(app, renderPass)
{
    mHasChanged = false;
}

PersistantNode::~PersistantNode()
{
    //dtor
}

bool PersistantNode::hasChanged()
{
    return mHasChanged;
}
