#include "Texture.h"

#include "stb_image.h"

Texture::Texture(GLenum target)
{
    mTarget = target;
    mInitialised = false;
    mID = 0;
}

Texture::~Texture()
{
    //dtor
}

void Texture::init()
{
    if(!mInitialised)
    {
        glGenTextures(1, &mID);
        mInitialised = true;
    }
}

void Texture::bind() const
{
    glBindTexture(mTarget, mID);
}

void Texture::unbind() const
{
    glBindTexture(mTarget, 0);
}

bool Texture::isBound() const
{
    return false;
}

GLuint Texture::getID()
{
    return mID;
}

void Texture::setMinFilter(GLenum minFilter)
{
    glTexParameterf(mTarget, GL_TEXTURE_MIN_FILTER, minFilter);
}

void Texture::setMaxFilter(GLenum maxFilter)
{
    glTexParameterf(mTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
}

void Texture::setWrapS(GLenum wrapS)
{
    glTexParameterf(mTarget, GL_TEXTURE_WRAP_S, wrapS);
}

void Texture::setWrapT(GLenum wrapT)
{
    glTexParameterf(mTarget, GL_TEXTURE_WRAP_T, wrapT);
}

bool Texture::loadImage(std::string filename)
{
    if(!mInitialised) return false;

    int width = 0;
    int height = 0;
    int numComponents = 0;
    //unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4); //Force 4 components.
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
    const char* debug = stbi_failure_reason();

    if(data == NULL)
    {
        return false;
    }

    bind();

        glTexImage2D(mTarget, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); checkError();

    unbind();

    stbi_image_free(data);

    return true;
}

 void Texture::generateMipMaps(unsigned int numMipMaps)
 {
     glTexParameterf(mTarget, GL_TEXTURE_MAX_LEVEL, numMipMaps);
     glGenerateMipmap(mTarget);
 }
