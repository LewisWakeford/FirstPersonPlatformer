#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <Buffer.h>

/*
    Class: ArrayBuffer
    A sub class of buffer that contains functionality for sub-arrays.
    Contains an internal class to manage arrays, which can be used by VAOs to bind attributes.
*/
class ArrayBuffer : public Buffer
{
    public:
        ArrayBuffer(GLenum type);
        virtual ~ArrayBuffer();

        class Array
        {
            public:
                unsigned int mName; //The name of this array, for future reference.
                unsigned int mElementsPerVertex; //Order of this array in the bufffer.
                unsigned int mElementType; //Size of each object in the array. This is each float or byte for example, not each entire structure.
                unsigned int mStride; //Bytes to skip between vertices.
                unsigned int mOffset; //Offset from the start of the array.
        };

        //Creates or overwrites an array with name: arrayName.
        void setArray(unsigned int arrayName, unsigned int elementsPerVertex, unsigned int elementType, unsigned int stride, unsigned int offset, unsigned int vertexAttrib);

        //Returns the array with name: arrayName or null if not found.
        const Array* getArray(unsigned int arrayName) const;

        void clearArrays();

    protected:



    std::vector<Array> mArrays;

    private:
};

#endif // VERTEXBUFFER_H
