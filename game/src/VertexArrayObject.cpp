#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    //ctor
}

VertexArrayObject::~VertexArrayObject()
{
    //dtor
}

void VertexArrayObject::init()
{
    glGenVertexArrays(1, &mID);
}

void VertexArrayObject::bind()
{
    glBindVertexArray(mID);
}

void VertexArrayObject::unbind()
{
    glBindVertexArray(0);
}

void VertexArrayObject::linkVertexArray(unsigned int attribIndex, ArrayBuffer* buffer, const unsigned int arrayName)
{
    glEnableVertexAttribArray(attribIndex);
    buffer->bind();
    const ArrayBuffer::Array* array = buffer->getArray(arrayName);
    glVertexAttribPointer(attribIndex, array->mElementsPerVertex, array->mElementType, GL_FALSE, array->mStride, (const GLvoid*)array->mOffset);
    buffer->unbind();
}
