#include "InputMapping.h"

#include "ActionSet.h"

InputMapping::InputMapping()
{
    //A - Move Left
    mMapping[65] = ACTION_B_MV_LF;

    //R - Move Right
    mMapping[68] = ACTION_B_MV_RI;

    //W - Move Forward
    mMapping[87] = ACTION_B_MV_FW;

    //S - Move Backward
    mMapping[83] = ACTION_B_MV_BW;

    //Up Cursor - Pitch Up
    mMapping[GLFW_KEY_UP] = ACTION_B_PITCH_P;

    //Down Cursor - Pitch Down
    mMapping[GLFW_KEY_DOWN] = ACTION_B_PITCH_N;

    //Left Cursor - Yaw Left
    mMapping[GLFW_KEY_LEFT] = ACTION_B_YAW_P;

    //Right Cursor - Yaw Right
    mMapping[GLFW_KEY_RIGHT] = ACTION_B_YAW_N;

    //Space - Move Up
    mMapping[GLFW_KEY_SPACE] = ACTION_B_MV_UP;

    //Ctrl - Move Down
    mMapping[GLFW_KEY_LCTRL] = ACTION_B_MV_DW;

    //Q - Roll Counter Clockwise
    mMapping[81] = ACTION_B_ROLL_P;

    //E - Roll Clockwise
    mMapping[69] = ACTION_B_ROLL_N;

    mMapping[GLFW_MOUSE_BUTTON_2] = ACTION_B_LEAP;

    mMouseXAxis = ACTION_D_AIM_H;
    mMouseYAxis = ACTION_D_AIM_V;

}

InputMapping::~InputMapping()
{
    //dtor
}

void InputMapping::setBind(int glfwKeyCode, unsigned int action)
{
    mMapping[glfwKeyCode] = action;
    unsigned int temp = mMapping[glfwKeyCode];
}

void InputMapping::setMouseXAxis(unsigned int action)
{
    mMouseXAxis = action;
}

void InputMapping::setMouseYAxis(unsigned int action)
{
    mMouseYAxis = action;
}

unsigned int InputMapping::getMouseXAxis()
{
    return mMouseXAxis;
}

unsigned int InputMapping::getMouseYAxis()
{
    return mMouseYAxis;
}

void InputMapping::load(std::string bindingFilename)
{
    //DERP
}

void InputMapping::save(std::string bindingFilename)
{
    //DERP
}

unsigned int InputMapping::getAction(int glfwKeyCode)
{
    unsigned int temp = mMapping[glfwKeyCode];
    return temp;
}
