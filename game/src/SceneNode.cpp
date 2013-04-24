#include "SceneNode.h"
#include "Renderer.h"
#include "Collider.h"
#include "App.h"
#include "CollisionSystem.h"

#include <iostream>

SceneNode::SceneNode(App* app, GLenum renderPass)
{
    mApp = app;
    mRenderPass = renderPass;
    mDeleted = false;
}

SceneNode::~SceneNode()
{
    for(unsigned int i = 0; i < mColliderList.size(); i++)
    {
        delete mColliderList[i];
    }
}

void SceneNode::onEvent()
{

}

void SceneNode::onInit()
{

}

void SceneNode::onLoop(GLdouble deltaTime)
{
    if(!mDeleted)
    {
        //Sublclasses would perform some kind of simulation here.
        //The superclass doesn't bother.
        simulateSelf(deltaTime);

        mApp->getCollisionSystem()->pushMatrix(mTransform);
        setAbsoluteTransform(mApp->getCollisionSystem()->currentMatrix());

        pushColliders();

        //Perform simulation for all children.
        for(unsigned int i = 0; i < mChildList.size(); i++)
        {
            mChildList[i]->onLoop(deltaTime);
        }
        mApp->getCollisionSystem()->popMatrix();
    }
}

void SceneNode::onRender(GLenum renderPass)
{
    if(!mDeleted)
    {
        //Push Matrix onto renderer's stack.
        mApp->getRenderer()->pushMatrix(getTransform());


    if(mRenderPass == renderPass)
    {
        //Subclasses would actually render themselves here.
        //The super class doesn't bother.
        checkError();
        renderSelf();
        checkError();
    }

    //Render all Children.
    for(unsigned int i = 0; i < mChildList.size(); i++)
    {
        SceneNodePtr child = mChildList[i];
        if(child->isDeleted())
        {
            mChildList.erase(mChildList.begin()+i);
        }
        else
        {
            child->onRender(renderPass);
        }
    }

    //Pop Matrix off renderer's stack.
    mApp->getRenderer()->popMatrix();
    }
}

void SceneNode::addCollider(Collider* collider)
{
    mColliderList.push_back(collider);
}

void SceneNode::pushColliders()
{
    for(unsigned int i = 0; i < mColliderList.size(); i++)
    {
        mApp->getCollisionSystem()->pushCollider(mColliderList[i]);
    }
}

void SceneNode::onCleanup()
{

}

void SceneNode::setTransform(glm::mat4 transform)
{
    mTransform = transform;
}

void SceneNode::setAbsoluteTransform(glm::mat4 transform)
{
    mAbsoluteTransform = transform;
}

glm::mat4 SceneNode::getTransform() const
{
    return mTransform;
}

void SceneNode::renderSelf()
{
    //Do Nothing
}

void SceneNode::simulateSelf(GLdouble deltaTime)
{

}

void SceneNode::addChild(SceneNodePtr child)
{
    mChildList.push_back(child);
}

GLboolean SceneNode::isDeleted()
{
    return mDeleted;
}

void SceneNode::deleteNode()
{
    mDeleted = true;
}

void SceneNode::onCollision(CollisionEvent event)
{
    //Do nothing
}
