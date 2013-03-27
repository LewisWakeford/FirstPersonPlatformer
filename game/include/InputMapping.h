#ifndef INPUTMAPPING_H
#define INPUTMAPPING_H

#include "InputMapping.h"

#include <string>
#include <map>

/*
    Class: Input Mapping
    Maps GLFW Key codes to my actions.
*/
class InputMapping
{
    public:
        InputMapping(); //Default Constructor uses default keybinds.
        InputMapping(std::string bindingFilename); //Load bindings from a file.
        virtual ~InputMapping();

        void setBind(int glfwKeyCode, unsigned int action);
        void setMouseXAxis(unsigned int action);
        void setMouseYAxis(unsigned int action);

        unsigned int getMouseXAxis();
        unsigned int getMouseYAxis();

        unsigned int getAction(int glfwKeyCode);

        void load(std::string bindingFilename);
        void save(std::string bindingFilename);

    protected:

        std::map<int, unsigned int> mMapping;

        unsigned int mMouseXAxis;
        unsigned int mMouseYAxis;

    private:
};

#endif // INPUTMAPPING_H
