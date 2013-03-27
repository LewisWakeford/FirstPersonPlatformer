#include "LocalHardwareInput.h"

#include "ActionSet.h"
#include "InputMapping.h"

LocalHardwareInput::LocalHardwareInput()
{
    mMouseX = 0;
    mMouseY = 0;
    mInitialized = false;
}

LocalHardwareInput::~LocalHardwareInput()
{
    //dtor
}

void LocalHardwareInput::setMapping(InputMapping* mapping)
{
    mMapping = mapping;
}

void LocalHardwareInput::setActionSet(ActionSet* actionSet)
{
    mActionSet = actionSet;
}

void LocalHardwareInput::keyCallback(int key, int action)
{
    if(mMapping && mActionSet)
    {
        unsigned int gameAction = mMapping->getAction(key);
        if(action == GLFW_PRESS)
        {
            mActionSet->setB(gameAction, true);
        }
        else
        {
            mActionSet->setB(gameAction, false);
        }
    }
}

void LocalHardwareInput::mouseCallback(int button, int action)
{
    if(mMapping && mActionSet)
    {
        unsigned int gameAction = mMapping->getAction(button);
        if(action == GLFW_PRESS)
        {
            mActionSet->setB(gameAction, true);
        }
        else
        {
            mActionSet->setB(gameAction, false);
        }
    }
}

void LocalHardwareInput::updateMouse()
{
    int newMouseX;
    int newMouseY;

    glfwGetMousePos(&newMouseX, &newMouseY);

    if(!mInitialized)
    {
        mMouseX = newMouseX;
        mMouseY = newMouseY;
        mInitialized = true;
    }

    int deltaX = newMouseX - mMouseX;
    int deltaY = newMouseY - mMouseY;

    unsigned int mouseActionX = mMapping->getMouseXAxis();
    unsigned int mouseActionY = mMapping->getMouseYAxis();


        mActionSet->setD(mouseActionX, -deltaX);
        mActionSet->setD(mouseActionY, -deltaY);


    mMouseX = newMouseX;
    mMouseY = newMouseY;
}
