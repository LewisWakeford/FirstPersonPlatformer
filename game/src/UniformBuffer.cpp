#include "UniformBuffer.h"

#include "ShaderProgram.h"

UniformBuffer::UniformBuffer() : Buffer(GL_UNIFORM_BUFFER)
{
    //ctor
}

UniformBuffer::~UniformBuffer()
{
    //dtor
}

void UniformBuffer::prepare(ShaderProgram* program, const std::string&uniformBlockName)
{
    //Buffer already genereated and bound.

    mBlockIndex = program->getUniformBlockIndex(uniformBlockName);
    mProgram = program;
}

bool UniformBuffer::ready()
{
    bool valid = true;
    for(unsigned int i = 0; i < mUniforms.size() && valid; i++)
    {
        valid = mUniforms[i].isSet;
    }
    return valid;
}

void UniformBuffer::linkToBlock(GLuint bindingIndex)
{
    if(ready())
    {
        bind();

        mProgram->bindBlock(mBlockIndex, bindingIndex);

        glBindBufferRange(mType, bindingIndex, mName, 0, mTotalSize);

        unbind();
    }
}

void UniformBuffer::specifyUniform(std::string name, GLuint offset, GLuint size)
{
    mUniforms.push_back(UniformLayout(name, offset, size));
    mTotalSize += size;
}

void UniformBuffer::setUniformValue(std::string name, const void* valuePointer)
{
    UniformLayout* layout;
    for(size_t i = 0; i < mUniforms.size(); i++)
    {
        if(name == mUniforms[i].uniformName)
        {
            layout = &mUniforms[i];
        }
    }
    if(layout == 0) return;
    layout->isSet = true;
    setSubData(valuePointer, layout->offset, layout->size);
}
