#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "SceneNode.h"

class Jumpman;
class VertexArrayObject;
class Buffer;
class ShaderProgram;

class Crosshair : public SceneNode
{
    public:
        Crosshair(App* app, GLfloat xPos, GLfloat yPos, GLfloat scale, GLfloat thickness, glm::vec4 color);
        virtual ~Crosshair();

        virtual void renderSelf();
        virtual void simulateSelf(double deltaTime);

    protected:
        //Screen space coords
        GLfloat mXPos; //Center of crosshair
        GLfloat mYPos;
        GLfloat mScale; //Distance stretched from center.
        GLfloat mThickness; //Thickness of crosshair lines.

        VertexArrayObject* mVAO;
        Buffer* mIndexBuffer;
        ShaderProgram* mProgram;
        glm::vec4 mColor;
        bool mInitilaized;

        void init();

    private:
};

#endif // CROSSHAIR_H
