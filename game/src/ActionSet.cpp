#include "ActionSet.h"

ActionSet::ActionSet() : mBoolActions(ACTION_B_COUNT__, false), mDoubleActions(ACTION_D_COUNT__, 0.0)
{
    //ctor
}

ActionSet::~ActionSet()
{
    //dtor
}

void ActionSet::setB(unsigned int actionEnum, bool value)
{
    mBoolActions[actionEnum] = value;
}

void ActionSet::setD(unsigned int actionEnum, double value)
{
    mDoubleActions[actionEnum] = value;
}

bool ActionSet::getB(unsigned int actionEnum) const
{
    return mBoolActions[actionEnum];
}

double ActionSet::getD(unsigned int actionEnum) const
{
    return mDoubleActions[actionEnum];
}
