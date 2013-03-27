#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "game.h"

class Shader
{
    public:
        Shader(const GLenum shaderType);
        virtual ~Shader();

        /*
            Function setShaderSource
            Set the source of a shader object.
        */
        void setSource(std::string shaderString);
        void setSource(const char* shaderString, int length);

        void load(const char* filename);

        /*
            Function compileShader
            Compile a shader object. Returns true if the shader compilation was successful and false if it failed. infoLog will contain any warnings or errors regardless of status.
        */
        bool compile(std::string* infoLog);

        GLuint getID();

    protected:

        GLuint mID;
        GLenum mType;
        GLboolean mCompiled;
    private:
};

#endif // SHADER_H
