#ifndef ShaderProgram_H
#define ShaderProgram_H

#include <string>

#include "game.h"

#include "Shader.h"

class Texture;

class ShaderProgram
{
    public:
        ShaderProgram();
        virtual ~ShaderProgram();

        void setVertexShader(Shader* vertexShader);
        void setFragmentShader(Shader* fragmentShader);

        bool link(std::string* infoLog);
        void use();

        GLuint getUniformBlockIndex(std::string uniformBlockName);
        void bindBlock(GLuint blockIndex, GLuint bindingIndex);

        GLint getAttribLocation(std::string);

        void setUniform1i(std::string uniformName, const GLint value);
        void setUniform1ui(std::string uniformName, const GLuint value);
        void setUniform1f(std::string uniformName, const GLfloat value);
        void setUniform3f(std::string uniformName, const GLfloat* valuePtr);
        void setUniform4f(std::string uniformName, const GLfloat* valuePtr);
        void setUniformMatrix3fv(std::string uniformName, const GLfloat* valuePtr);
        void setUniformMatrix4fv(std::string uniformName, const GLfloat* valuePtr);
        void setVertexAttribPointer(std::string attribName,
                            GLint  	size,
                            GLenum  	type,
                            GLboolean  	normalized,
                            GLsizei  	stride,
                            const GLvoid* ptr);

        void setTexture(Texture* texture);

    protected:
        GLuint mID;

        Shader* mVertexShader;
        Shader* mFragmentShader;
        Texture* mTexture;

        GLboolean mLinked;

    private:
};

#endif // ShaderProgram_H
