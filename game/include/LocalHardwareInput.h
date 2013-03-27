#ifndef LOCALHARDWAREINPUT_H
#define LOCALHARDWAREINPUT_H

class InputMapping;
class ActionSet;

class LocalHardwareInput
{
    public:
        LocalHardwareInput();
        virtual ~LocalHardwareInput();

        void keyCallback(int key, int action);
        void mouseCallback(int button, int action);

        void setMapping(InputMapping* mapping);
        void setActionSet(ActionSet* actionSet);

        void updateMouse();

    protected:

        int mMouseX;
        int mMouseY;

        bool mInitialized;

        InputMapping* mMapping;
        ActionSet* mActionSet;

    private:
};

#endif // LOCALHARDWAREINPUT_H
