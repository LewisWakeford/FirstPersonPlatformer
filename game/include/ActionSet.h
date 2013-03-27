#ifndef ACTIONSET_H
#define ACTIONSET_H

#include <vector>

#include "game.h"

//Boolean Actions
enum ACTIONB
{
    ACTION_B_MV_FW,
    ACTION_B_MV_BW,
    ACTION_B_MV_LF,
    ACTION_B_MV_RI,
    ACTION_B_MV_UP,
    ACTION_B_MV_DW,
    ACTION_B_PITCH_P,
    ACTION_B_PITCH_N,
    ACTION_B_YAW_P,
    ACTION_B_YAW_N,
    ACTION_B_ROLL_P,
    ACTION_B_ROLL_N,
    ACTION_B_JUMP,
    ACTION_B_DUCK,
    ACTION_B_LEAP,
    ACTION_B_CLIMB,

    ACTION_B_COUNT__ //Should contain the number of items in this enum.
};

//Double Actions
enum ACTIOND
{
    ACTION_D_AIM_H,
    ACTION_D_AIM_V,

    ACTION_D_COUNT__ //Should contain the number of items in this enum.
};

/*
    Class: ActionSet
    Any driver type object should use an action set to seperate the input (maybe from a node instead of keyboard) from the actual performing of the actions.
    In the case of the player it allows for rebindable keys.
*/

class ActionSet
{
    public:
        ActionSet();
        virtual ~ActionSet();

        void setB(unsigned int actionEnum, bool value);
        void setD(unsigned int actionEnum, double value);
        bool getB(unsigned int actionEnum) const;
        double getD(unsigned int actionEnum) const;

    protected:

        std::vector<bool> mBoolActions;
        std::vector<double> mDoubleActions;

    private:
};

#endif // ACTIONSET_H
