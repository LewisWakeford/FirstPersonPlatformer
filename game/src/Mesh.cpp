#include "Mesh.h"

#include "glm/gtc/type_ptr.hpp"

#include "VertexArrayObject.h"
#include "UniformBuffer.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Texture.h"

Mesh::Mesh()
{
    mVertexArrayObject = 0;
    mIndexBuffer = 0;
    mUniformBuffer = 0;
    mShaderProgram = 0;
    mMaterial = 0;
}

Mesh::~Mesh()
{
    //dtor
}

void Mesh::render(glm::mat4 MVP, glm::mat3 normal) const
{
    checkError();
    if(mVertexArrayObject != 0 && mIndexBuffer != 0)
    {
        mVertexArrayObject->bind();checkError();
        mIndexBuffer->bind();checkError();

        const GLfloat* matrixPtr = glm::value_ptr(MVP);checkError();
        const GLfloat* normalMatrixPtr = glm::value_ptr(normal);

        if(mShaderProgram != 0)
        {
            mShaderProgram->use();

            if(mMaterial != 0)
            {
                Texture* texture = mMaterial->getTexture();

                if(texture != 0)
                {
                    glActiveTexture(GL_TEXTURE0);
                    texture->bind();
                }
            }

            mShaderProgram->setUniformMatrix4fv("m_MVP", matrixPtr);checkError();
            mShaderProgram->setUniformMatrix3fv("m_normal", normalMatrixPtr);checkError();
            mShaderProgram->setUniform3f("v3_world_light_diffuse", glm::value_ptr(glm::vec3(0.6f, 0.6f, 0.6f)));checkError();
            mShaderProgram->setUniform3f("v3_world_light_ambient", glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));checkError();
            mShaderProgram->setUniform3f("v3_world_light_vector", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));checkError();

            checkError();
        }


        //GLfloat* verts = (GLfloat*)glMapBufferRange(GL_ARRAY_BUFFER, 0, (sizeof(GLfloat)*8*3), GL_MAP_READ_BIT);
        //GLuint* ind = (GLuint*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, (4*6*2*3), GL_MAP_READ_BIT);

        if(mUniformBuffer) mUniformBuffer->linkToBlock(0);

        glDrawElements(GL_TRIANGLES, mIndiceCount, GL_UNSIGNED_INT, 0);
        checkError();

        mVertexArrayObject->unbind();
    }
}

void Mesh::setVAO(VertexArrayObject* vao)
{
    mVertexArrayObject = vao;
}

void Mesh::setProgram(ShaderProgram* shaderProgram)
{
    mShaderProgram = shaderProgram;
}

void Mesh::setMaterial(Material* material)
{
    mMaterial = material;
}

void Mesh::setIndexBuffer(Buffer* indexBuffer)
{
    mIndexBuffer = indexBuffer;
}

void Mesh::setUniformBuffer(UniformBuffer* uniformBuffer)
{
    mUniformBuffer = uniformBuffer;
}

void Mesh::setNumIndices(GLuint indiceCount)
{
    mIndiceCount = indiceCount;
}


