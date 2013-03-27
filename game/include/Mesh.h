#ifndef MESH_H
#define MESH_H

#include <vector>

#include "game.h"

#include <glm/glm.hpp>

class VertexArrayObject;
class UniformBuffer;
class ShaderProgram;
class Material;
class Buffer;

/*
    Class: Mesh
    Links a VAO, a shader program, the uniforms for the program, and a material.
*/
class Mesh
{
    public:

        //Render the mesh with the MVP matrix.
        void render(glm::mat4 MVP) const;

        void setVAO(VertexArrayObject* vao);
        void setNumIndices(GLuint indiceCount);
        void setIndexBuffer(Buffer* indexBuffer);
        void setUniformBuffer(UniformBuffer* uniformBuffer);
        void setProgram(ShaderProgram* shaderProgram);
        void setMaterial(Material* material);

        void loadFromObj(std::string filename);

        Mesh();
        virtual ~Mesh();

    protected:

        VertexArrayObject* mVertexArrayObject;
        Buffer* mIndexBuffer;
        UniformBuffer* mUniformBuffer;
        ShaderProgram* mShaderProgram;
        Material* mMaterial;

        GLuint mIndiceCount;


    private:



        friend class ResourceManager;
};

#endif // MESH_H
