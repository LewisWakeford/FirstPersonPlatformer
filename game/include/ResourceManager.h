#ifndef RESOURSEMANAGER_H
#define RESOURSEMANAGER_H

#include <vector>
#include <unordered_map>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "VertexArrayObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Buffer.h"
#include "ArrayBuffer.h"
#include "UniformBuffer.h"
#include "Cache.h"

//Class: ResourseManager
//Description: Loads and stores resources or references to resources such as meshes or textures.
//All resources need a reference string (might hash for optimization) that can be used to retrieve them. Resources loaded from files should use the file name as a reference string.
//Upon instantiation creates a set of test objects. These objects will be defaulted to when an object tries to use an invalid resource.
//For example rendering with a missing mesh will produce the debug cube.

class ResourceManager
{
    public:
        ResourceManager();
        virtual ~ResourceManager();

        bool init(std::string* infoLog);

        Shader* createShader(GLenum shaderType, std::string ref);
        Shader* createShaderFromFile(GLenum shaderType, std::string filename);
        ShaderProgram* createProgram(std::string ref);
        ShaderProgram* createProgramFromFiles(std::string ref, std::string vertexShaderFilename, std::string fragmentShaderFilename);
        Material* createMaterial(std::string ref);
        Material* loadMaterialFromMtl(std::string filename);
        VertexArrayObject* createVAO(std::string ref);
        Mesh* createMesh(std::string ref); //Create a new mesh object, store it in the cache and return a reference.
        Mesh* loadMeshFromObj(std::string filename, ShaderProgram* program);
        Texture* createTextureFromFile(std::string filename); //Textures are loaded from files, so filename and ref are the same.
        Texture* createTexture(std::string, GLenum target); //Create a blank texture, just in case I need to do something fancy.

        Buffer* createBuffer(GLenum bufferType, std::string ref);
        ArrayBuffer* createArrayBuffer(GLenum bufferType, std::string ref);
        UniformBuffer* createUniformBuffer(std::string ref);

        Shader* getShader(std::string ref);
        ShaderProgram* getProgram(std::string ref);
        Material* getMaterial(std::string ref);
        VertexArrayObject* getVAO(std::string ref);
        Mesh* getMesh(std::string ref);
        Texture* getTexture(std::string ref);

        Buffer* getBuffer(std::string ref);
        ArrayBuffer* getArrayBuffer(std::string ref);
        UniformBuffer* getUniformBuffer(std::string ref);


    protected:

        ShaderProgram* defaultProgram;
        Material* defaultMaterial;
        Mesh* defaultMesh;

        Cache<Shader> mShaderCache;
        Cache<ShaderProgram> mProgramCache;
        Cache<Material> mMaterialCache;
        Cache<VertexArrayObject> mVaoCache;
        Cache<Mesh> mMeshCache;
        Cache<Texture> mTextureCache;

        Cache<Buffer> mBufferCache;
        Cache<ArrayBuffer> mArrayBufferCache;
        Cache<UniformBuffer> mUniformBufferCache;


    private:
};

#endif // RESOURSEMANAGER_H
