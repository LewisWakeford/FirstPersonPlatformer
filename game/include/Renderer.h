#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "game.h"

#include <glm/glm.hpp>

class Mesh;
class Billboard;
class CameraNode;

class Renderer
{
    public:
        Renderer();
        virtual ~Renderer();

        void pushMatrix(glm::mat4 matrix);
        glm::mat4 popMatrix();
        glm::mat4 currentMatrix();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        GLdouble getDisplayWidth();
        GLdouble getDisplayHeight();

        void setDimensions(GLdouble width, GLdouble height);
        void setCamera(CameraNode* camera);
        void updateCamera(CameraNode* camera);

    protected:

        std::vector<glm::mat4> mMatrixStack;

        CameraNode* mCurrentCamera;
        glm::mat4 mViewMatrix;
        glm::mat4 mProjectionMatrix;

        GLdouble mViewportHeight;
        GLdouble mViewportWidth;


    private:
};

#endif // RENDERER_H
