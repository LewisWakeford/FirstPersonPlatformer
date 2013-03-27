#include "ShaderProgram.h"
#include "Texture.h"

ShaderProgram::ShaderProgram()
{
    mID = glCreateProgram();
    mTexture = 0;
}

ShaderProgram::~ShaderProgram()
{
    //dtor
}

void ShaderProgram::setVertexShader(Shader* vertexShader)
{
    mVertexShader = vertexShader;
    glAttachShader(mID, mVertexShader->getID());
}

void ShaderProgram::setFragmentShader(Shader* fragmentShader)
{
    mFragmentShader = fragmentShader;
    glAttachShader(mID, mFragmentShader->getID());
}

bool ShaderProgram::link(std::string* infoLog)
{
    glLinkProgram(mID); checkError();

    GLint linkStatus;
	glGetProgramiv(mID, GL_LINK_STATUS, &linkStatus); checkError();

	//Retrieve info log
	GLint length;
	glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &length); checkError();

    char* c_infoLog = new char[length];

	glGetProgramInfoLog(mID, length, NULL, c_infoLog); checkError();

	infoLog->assign(c_infoLog, length);

	//If linking failed then return false
	if(linkStatus == GL_FALSE)
	{
		return false;
	}
	//Otherwise return true.
	else
	{
		mLinked = GL_TRUE;

		//If linking worked, set the textures to point to the correct places:
		use();
        GLint textureLocation0 = glGetUniformLocation(mID, "tex2D_texture0"); checkError();
        if(textureLocation0 >= 0)
        {
            glUniform1i(textureLocation0, 0); checkError();
        }

		return true;
	}
}


void ShaderProgram::use()
{
	glUseProgram(mID);
}

void ShaderProgram::setUniform1i(std::string uniformName, const GLint value)
{
    GLint location = glGetUniformLocation(mID, uniformName.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setUniform1ui(std::string uniformName, const GLuint value)
{
    GLint location = glGetUniformLocation(mID, uniformName.c_str());
    glUniform1ui(location, value);
}

void ShaderProgram::setUniform1f(std::string uniformName, const GLfloat value)
{
    GLint location = glGetUniformLocation(mID, uniformName.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::setUniform4f(std::string uniformName, const GLfloat* valuePtr)
{
    GLint location = glGetUniformLocation(mID, uniformName.c_str());
    glUniform4fv(location, 1, valuePtr);
}

void ShaderProgram::setUniformMatrix4fv(std::string uniformName, const GLfloat* valuePtr)
{
    GLint location = glGetUniformLocation(mID, uniformName.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, valuePtr);
}

void ShaderProgram::setVertexAttribPointer(std::string attribName,
                            GLint  	size,
                            GLenum  	type,
                            GLboolean  	normalized,
                            GLsizei  	stride,
                            const GLvoid* ptr)
{
    GLint location = glGetAttribLocation(mID, attribName.c_str());
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, normalized, stride, ptr);
}

GLuint ShaderProgram::getUniformBlockIndex(std::string uniformBlockName)
{
    return glGetUniformBlockIndex(mID, uniformBlockName.c_str());
}

void ShaderProgram::bindBlock(GLuint blockIndex, GLuint bindingIndex)
{
    glUniformBlockBinding(mID, blockIndex, bindingIndex);
}

GLuint ShaderProgram::getAttribLocation(std::string attribName)
{
    return glGetAttribLocation(mID, attribName.c_str());
}

void ShaderProgram::setTexture(Texture* texture)
{
    mTexture = texture;
}
