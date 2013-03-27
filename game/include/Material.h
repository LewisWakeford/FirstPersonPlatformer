#ifndef MATERIAL_H
#define MATERIAL_H

#include "game.h"

#include <string>

#include "glm/glm.hpp"

class Texture;

class Material
{
    public:
        Material();
        virtual ~Material();

        void setTexture(Texture* texture);
        void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        void setName(std::string name);

        Texture* getTexture();
        glm::vec4 getAmbient();
        glm::vec4 getDiffuse();
        glm::vec4 getSpecular();
        std::string getName();

        void loadFromMtl(std::string filename);

    protected:
        Texture* mTexture;
        glm::vec4 mAmbient; //Ambient Reflectivity.
        glm::vec4 mDiffuse; //Diffuse Reflectivity. Added to texture color.
        glm::vec4 mSpecular; //Specular Reflectivity.
        std::string mName;


    private:
};

#endif // MATERIAL_H
