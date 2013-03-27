#ifndef TEXTURE_H
#define TEXTURE_H

#include "game.h"

#include <string>

#include "Cache.h"

class Texture
{
    public:

        void init();
        void bind() const;
        void unbind() const;
        bool isBound() const; //Check texture is bound.

        void generateMipMaps(unsigned int numMipMaps);
        void setMinFilter(GLenum minFilter);
        void setMaxFilter(GLenum maxFilter);
        void setWrapS(GLenum wrapS);
        void setWrapT(GLenum wrapT);

        GLuint getID();

        bool loadImage(std::string filename);

    protected:

        GLuint mID;
        GLenum mTarget;
        GLenum mMinFilter;
        GLenum mMaxFilter;
        GLenum mWrapS;
        GLenum mWrapT;

        friend class ResourceManager;

        friend class Cache<Texture>;

        Texture(GLenum target);
        virtual ~Texture();



    private:

        bool mInitialised;
};

#endif // TEXTURE_H
