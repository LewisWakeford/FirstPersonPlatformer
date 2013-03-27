#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include "game.h"

#include "ArrayBuffer.h"

/*
    Class: VertexArrayObject
    Manages a VAO in OpenGL.
*/
class VertexArrayObject
{
    public:
        VertexArrayObject();
        virtual ~VertexArrayObject();

        void init();
        void bind();
        void unbind();

        void linkVertexArray(unsigned int attribIndex, ArrayBuffer* buffer, const unsigned int arrayName);

    protected:

        GLuint mID;

    private:
};

#endif // VERTEXARRAYOBJECT_H
