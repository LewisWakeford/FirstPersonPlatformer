#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "Buffer.h"

#include <string>

class ShaderProgram;

class UniformBuffer : public Buffer
{
    public:
        UniformBuffer();
        virtual ~UniformBuffer();

        //Gathers all the information needed for bind() method.
        void prepare(ShaderProgram* program, const std::string& uniformBlockName);

        //Links this block with the shader block.
        void linkToBlock(GLuint bindingIndex);

        void specifyUniform(std::string name, GLuint offset, GLuint size);
        void setUniformValue(std::string name, const void* valuePointer);
        bool ready(); //Returns true if all uniforms in the block are specified.

    protected:

        class UniformLayout
        {
            public:
                UniformLayout(std::string n, GLuint o, GLuint s)
                {
                    uniformName = n;
                    offset = o;
                    size = s;
                    isSet = false;
                }

                std::string uniformName;
                GLuint offset;
                GLuint size;

                bool isSet;
        };

        GLuint mBlockIndex;
        ShaderProgram* mProgram;

        std::vector<UniformLayout> mUniforms;


    private:
};

#endif // UNIFORMBUFFER_H
