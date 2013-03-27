#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define GLEW_STATIC ;
#include <GL/glew.h>
#include <GL/glfw.h>

#include <iostream>
#include <string>

inline std::string getErrorString(GLint errorCode)
{
    switch(errorCode)
    {
        case GL_NO_ERROR:
            return "No Error";
        break;
        case GL_INVALID_ENUM:
            return "Invalid Enum";
        break;
        case GL_INVALID_OPERATION:
            return "Invalid Operation";
        break;
        case GL_INVALID_VALUE:
            return "Invalid Value";
        break;
        case GL_STACK_OVERFLOW:
            return "Stack Overflow";
        break;
        case GL_STACK_UNDERFLOW:
            return "Stack Underflow";
        break;
        case GL_OUT_OF_MEMORY:
            return "Out of Memory";
        break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "Invalid Framebuffer Operation";
        break;
        case GL_TABLE_TOO_LARGE:
            return "Table too Large";
        break;
        default:
            return "Unrecognized Error";
    }
}

inline void checkError()
{
    GLint errorCode = glGetError();

    unsigned int maxErrors = 40;
    unsigned int errorCount = 0;

    while(errorCode != GL_NO_ERROR && errorCount < maxErrors)
    {
        std::cout << "GL ERROR: " << errorCode << " " << getErrorString(errorCode) << std::endl;
        errorCode = glGetError();
        errorCount++;
    }

    if(errorCount >= maxErrors)
    {
        std::cout << "GL ERROR: Too many OpenGL errors" << std::endl;
    }
};

//---CONSTANTS---

//Render Passes
enum
{
    GAME_RENDER_NONE,
    GAME_RENDER_GEOMETRY,
    GAME_RENDER_SKY,
    GAME_RENDER_UI_2D,
    GAME_RENDER_UI_3D,
};

//Collision Groups
enum
{
    GAME_COLLISION_NONE = 1,
    GAME_COLLISION_PLAYER = 2,
    GAME_COLLISION_BLOCK = 4
};

//Mesh Types
enum
{
    GAME_MESH_TEXTURED,
    GAME_MESH_COLORED
};

//---DATA STRUCTS---
struct xyz
{
    float x, y, z;
};

typedef xyz Vertex3, Point3, Vector3;

struct xy
{
    float x, y;
};

typedef xy Vertex2, Point2, Vector2;

#endif // GAME_H_INCLUDED
