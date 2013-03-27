#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include "game.h"

/*
    Class: Buffer
    Manages an OpenGL buffer object of any type.
*/
class Buffer
{
    public:
        Buffer(GLenum type);
        virtual ~Buffer();

        void init(); //Generate the buffer if necessary, clear if already inited.
         //Set the enitre buffer data.
         //itemSize is the size of each repeating struct in the buffer, while itemCount is the number of these structs.
         //Array buffers will probably have a small struct repeated many times, while unifrom buffers will just have one large struct.
        void setData(const void* dataPointer, unsigned int itemCount, unsigned int itemSize);
        void setSubData(const void* dataPointer, unsigned int offset, unsigned int size); //Set a chunk of buffer data, from offset to offset+size.

        void bind() const;
        void unbind() const;
        bool isBound() const; //Check buffer is bound.

        unsigned int getName() const; //Return the automatically generated buffer "name" (aka ID number).
        unsigned int getItemSize() const;
        unsigned int getTotalSize() const;

    protected:
        static int sBoundBuffer; //The currently bound buffer.

        unsigned int mName; //The "name" (ID number) of the VBO.

        unsigned int mItemCount; //The number of objects in the buffer.
        unsigned int mItemSize; //The size of the objects stored in the buffer, in bytes.
        unsigned int mTotalSize; //Running total of size.

        GLenum mType;

    private:
};

#endif // BUFFER_H
