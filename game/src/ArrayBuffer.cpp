#include "ArrayBuffer.h"

ArrayBuffer::ArrayBuffer(GLenum mType) : Buffer(mType)
{
    //ctor
}

ArrayBuffer::~ArrayBuffer()
{
    //dtor
}

void ArrayBuffer::setArray(unsigned int arrayName, unsigned int elementsPerVertex, unsigned int elementType, unsigned int stride, unsigned int offset, unsigned int vertexAttrib)
{
    Array array;
    array.mName = arrayName;
    array.mElementsPerVertex = elementsPerVertex;
    array.mElementType = elementType;
    array.mStride = stride;
    array.mOffset = offset;

    mArrays.push_back(array);
}

const ArrayBuffer::Array* ArrayBuffer::getArray(unsigned int arrayName) const
{
    for(size_t i = 0; i < mArrays.size(); i++)
    {
        if(mArrays[i].mName == arrayName)
        {
            return &mArrays[i];
        }
    }
    return 0;
}

void ArrayBuffer::clearArrays()
{
    mArrays.clear();
}
