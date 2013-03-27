#include "Material.h"

Material::Material()
{
    mTexture = 0;
}

Material::~Material()
{
    //dtor
}

void Material::setTexture(Texture* texture)
{
    mTexture = texture;
}

void Material::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    mAmbient[0] = r;
    mAmbient[1] = g;
    mAmbient[2] = b;
    mAmbient[3] = a;
}

void Material::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    mDiffuse[0] = r;
    mDiffuse[1] = g;
    mDiffuse[2] = b;
    mDiffuse[3] = a;
}

void Material::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    mSpecular[0] = r;
    mSpecular[1] = g;
    mSpecular[2] = b;
    mSpecular[3] = a;
}

void Material::setName(std::string name)
{
    mName = name;
}

Texture* Material::getTexture()
{
    return mTexture;
}

glm::vec4 Material::getAmbient()
{
    return mAmbient;
}

glm::vec4 Material::getDiffuse()
{
    return mDiffuse;
}

glm::vec4 Material::getSpecular()
{
    return mSpecular;
}

std::string Material::getName()
{
    return mName;
}

