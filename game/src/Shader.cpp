#include "Shader.h"

#include <iostream>
#include <fstream>

Shader::Shader(const GLenum shaderType)
{
    mID = glCreateShader(shaderType);
	mType = shaderType;
	mCompiled = GL_FALSE;
}

Shader::~Shader()
{
    //dtor
}

void Shader::load(const char* filename)
{
    std::ifstream shaderFile;
    shaderFile.open(filename, std::ios::binary);

    if(shaderFile.is_open())
    {
        std::streampos start = shaderFile.tellg();
        shaderFile.seekg (0, shaderFile.end);
        std::streamsize size = shaderFile.tellg() - start;
        char* shaderString = new char [size];
        shaderFile.seekg (0, shaderFile.beg);
        shaderFile.read (shaderString, size);
        shaderFile.close();
        setSource(shaderString, size);
        delete shaderString;
    }
}

/*
	Function setShaderSource
	Set the source of a shader object.
*/
void Shader::setSource(const char* shaderString, GLint length)
{
	glShaderSource(mID, 1, &shaderString, &length);
}

void Shader::setSource(std::string shaderString)
{
    setSource(shaderString.c_str(), shaderString.length());
}

/*
	Function compileShader
	Compile a shader object. Returns true if the shader compilation was successful and false if it failed. infoLog will contain any warnings or errors regardless of status.
*/
bool Shader::compile(std::string* infoLog)
{
	//Attempt Compilation.
	glCompileShader(mID);
	checkError();

	//Check Status.
	GLint compileStatus = GL_FALSE;
	glGetShaderiv(mID, GL_COMPILE_STATUS, &compileStatus);
	checkError();

	//Retrieve info log
	GLint length;
	glGetShaderiv(mID, GL_INFO_LOG_LENGTH, &length);
	checkError();

    char* c_infoLog = new char[length];

	glGetShaderInfoLog(mID, length, NULL, c_infoLog);
	checkError();
	infoLog->assign(c_infoLog, length);

	//If Compilation failed then return false
	if(compileStatus == GL_FALSE)
	{
		return false;
	}
	//Otherwise return true.
	else
	{
		mCompiled = GL_TRUE;
		return true;
	}
}

GLuint Shader::getID()
{
    return mID;
}
