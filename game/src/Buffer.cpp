#include "Buffer.h"

int Buffer::sBoundBuffer = -1;

Buffer::Buffer(GLenum type)
{
    mType = type;
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &mName);
}

void Buffer::init()
{
    glGenBuffers(1, &mName);
}

void Buffer::bind() const
{
        glBindBuffer(mType, mName);

        //mName will not be reliable as different types of buffer may have the same name.
        sBoundBuffer = mName;
}

void Buffer::unbind() const
{
    glBindBuffer(mType, 0);
    sBoundBuffer = 0;
}

bool Buffer::isBound() const
{
    return (sBoundBuffer == mName);
}

void Buffer::setData(const void* dataPointer, unsigned int itemCount, unsigned int itemSize)
{
    mItemCount = itemCount;
    mItemSize = itemSize;
    mTotalSize = itemCount * itemSize;

    glBufferData(mType, mTotalSize, dataPointer, GL_DYNAMIC_DRAW);
}

void Buffer::setSubData(const void* dataPointer, unsigned int offset, unsigned int size)
{
    glBufferSubData(mType, offset, size, dataPointer);
}

unsigned int Buffer::getName() const
{
    return mName;
}

unsigned int Buffer::getItemSize() const
{
    return mItemSize;
}

unsigned int Buffer::getTotalSize() const
{
    return mTotalSize;
}
