#ifndef I_CONTROLLABLE_H
#define I_CONTROLLABLE_H

class ActionSet;

/*
    Interface Class: Controllable
    Any class that can be controlled by the player should inherit this class.
    Any kind of driver (such as the player) will call processInput with a pointer to their current action set.
    ActionSet essentially contains the boolean values for each action such as ACTIONB_MV_FW or ACTIONB_JUMP as well as
    the Float based actions like ACTION_AIM_H or ACTIONF_AIM_V
*/
class I_Controllable
{
    public:
        I_Controllable();
        virtual ~I_Controllable();

        virtual void processInput(const ActionSet* actionSet) = 0;
    protected:
    private:
};

#endif // I_CONTROLLABLE_H
