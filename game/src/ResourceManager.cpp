#include "ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>

ResourceManager::ResourceManager()
{
    std::string infoLog;
    bool success = init(&infoLog);
    if(success)
    {
        std::cout << "Default Resources Initialised: " << std::endl;
    }
    else
    {
        std::cout << "Default Resources NOT Initialised: " << std::endl;
    }
    std::cout << infoLog << std::endl;
    std::cout.flush();
}

ResourceManager::~ResourceManager()
{
    mShaderCache.purge();
    mProgramCache.purge();
    mMaterialCache.purge();
    mVaoCache.purge();
    mMeshCache.purge();
    mTextureCache.purge();
    mBufferCache.purge();
    mArrayBufferCache.purge();
    mUniformBufferCache.purge();
}

bool ResourceManager::init(std::string* infoLog)
{
    //Create test/default objects.
    //Also a handy reminder to myself of how to do it.

    //Test Program: Debug Shader Program
    //No lighting, no textures. Color is allowed.


    Shader* debugVert = createShader(GL_VERTEX_SHADER, "debug_vertex");
    Shader* debugFrag = createShader(GL_FRAGMENT_SHADER, "debug_frag");
checkError();
    std::string vertexShaderString =
        "#version 330 \n"
        "\n"
        "in vec3 v3_position; \n"
        "uniform mat4 m_MVP; \n"
        "\n"
        "uniform MaterialBlock_BasicColor \n"
        "{ \n"
        "   vec4 v4_color; \n"
        "} material; \n"
        "\n"
        "smooth out vec4 v4_vertex_color; \n"
        "\n"
        "void main() \n"
        "{ \n"
        "   vec4 temp = vec4(v3_position, 1.0); \n"
        "   gl_Position = m_MVP * temp; \n"
        "   v4_vertex_color = material.v4_color; \n"
        "}";

    std::string vertexShaderInfoLog;
    debugVert->setSource(vertexShaderString);
    bool vertexShaderCompiled = debugVert->compile(&vertexShaderInfoLog);

    infoLog->append("Vertex Shader Info Log: \n");
    infoLog->append(vertexShaderInfoLog);
    if(!vertexShaderCompiled) return false;

    std::string fragmentShaderString =
        "#version 330 \n"
        "\n"
        "smooth in vec4 v4_vertex_color; \n"
        "\n"
        "out vec4 v4_fragment_color; \n"
        "\n"
        "void main() \n"
        "{ \n"
        "   v4_fragment_color = v4_vertex_color; \n"
        "}";


    std::string fragShaderInfoLog;
    debugFrag->setSource(fragmentShaderString);
    bool fragShaderCompiled = debugFrag->compile(&fragShaderInfoLog);
checkError();
    infoLog->append("Fragment Shader Info Log: \n");
    infoLog->append(fragShaderInfoLog);
    if(!fragShaderCompiled) return false;


    ShaderProgram* debugProgram = createProgram("debug");
    debugProgram->setVertexShader(debugVert);
    debugProgram->setFragmentShader(debugFrag);

    std::string programInfoLog;

    bool programLinked = debugProgram->link(&programInfoLog);
    infoLog->append("Program Link Info Log: \n");
    infoLog->append(programInfoLog);
    if(!programLinked) return false;
checkError();
    //Test Material: Debug Material
    //Pink and Black Square texture, just diffuse pink for now.

    Material* debugMaterial = createMaterial("debug");
    debugMaterial->setDiffuse(1.0f, 0.7f, 0.7f, 1.0f);


    //Test Mesh: Debug Cube
    //1x1x1 GLunit cube.

    Mesh* debugCube = createMesh("debug_cube");

checkError();
    GLfloat debugCube_Vertices[] =
    {
        -0.5f,  -0.5f,  -0.5f,  //left, bottom, back   - 0
        0.5f,   -0.5f,  -0.5f,  //right, bottom, back  - 1
        -0.5,   -0.5f,  0.5f,   //left, bottom, front  - 2
        0.5,    -0.5f,  0.5f,   //right, bottom, front - 3

        -0.5f,  0.5f,   -0.5f,  //left, top, back   - 4
        0.5f,   0.5f,   -0.5f,  //right, top, back  - 5
        -0.5,   0.5f,   0.5f,   //left, top, front  - 6
        0.5,    0.5f,   0.5f   //right, top, front  - 7
    };

    GLuint debugCube_Indices[] =
    {
        4, 0, 1,
        1, 5, 4,    //+Z Face

        5, 1, 3,
        3, 7, 5,    //+X Face

        7, 3, 2,
        2, 6, 7,    //-Z Face

        6, 2, 0,
        0, 4, 6,    //-X Face

        5, 7, 6,
        6, 4, 5,    //+Y Face

        3, 1, 0,
        0, 2, 3     //-Y Face
    };

    VertexArrayObject* vao = createVAO("debug_cube");
    vao->init();

    GLuint positionLocation = debugProgram->getAttribLocation("v3_position");

checkError();
        ArrayBuffer* vertexBuffer = createArrayBuffer(GL_ARRAY_BUFFER, "debug_cube_vertex_buffer");checkError();
        vertexBuffer->init();checkError();
        vertexBuffer->bind();
        vertexBuffer->setData(debugCube_Vertices, 8*3, sizeof(GLfloat));checkError();
        vertexBuffer->unbind();

        GLuint vertexArray = 0;checkError();
        vertexBuffer->setArray(vertexArray, 3, GL_FLOAT, 0, 0, positionLocation);checkError();

        vao->bind();
        vao->linkVertexArray(positionLocation, vertexBuffer, vertexArray);
        vao->unbind();

        Buffer* indexBuffer = createBuffer(GL_ELEMENT_ARRAY_BUFFER, "debug_cube_index_buffer");checkError();
        indexBuffer->init();checkError();
        indexBuffer->bind();
        indexBuffer->setData(debugCube_Indices, 6*2*3, sizeof(GLuint));checkError();
        indexBuffer->unbind();

        debugCube->setVAO(vao);checkError();
        debugCube->setIndexBuffer(indexBuffer);
        debugCube->setNumIndices(6*2*3);checkError();

    UniformBuffer * uniBuffer = createUniformBuffer("debug_cube_uniform_buffer");checkError();
    uniBuffer->init();
    uniBuffer->prepare(debugProgram, "MaterialBlock_BasicColor");checkError();
    uniBuffer->specifyUniform("v4_color", 0, sizeof(glm::vec4));checkError();
    uniBuffer->bind();
    uniBuffer->setData(0, 1, sizeof(glm::vec4));
    uniBuffer->setUniformValue("v4_color", glm::value_ptr(debugMaterial->getDiffuse()));checkError();
    uniBuffer->unbind();

    debugCube->setUniformBuffer(uniBuffer);

    debugCube->setMaterial(debugMaterial);
    debugCube->setProgram(debugProgram);

    defaultMesh = debugCube;
    defaultProgram = debugProgram;
    defaultMaterial = debugMaterial;

    return true;
}

Shader* ResourceManager::createShader(GLenum shaderType, std::string ref)
{
    Shader* shader = new Shader(shaderType);
    mShaderCache.add(ref, shader);
    return shader;
}

Shader* ResourceManager::createShaderFromFile(GLenum shaderType, std::string filename)
{
    Shader* shader = createShader(shaderType, filename);
    shader->load(filename.c_str());
    std::string info = "";
    if(!shader->compile(&info))
        std::cout << info <<std::endl;
    return shader;
}

ShaderProgram* ResourceManager::createProgram(std::string ref)
{
    ShaderProgram* program = new ShaderProgram();
    mProgramCache.add(ref, program);
    return program;
}

ShaderProgram* ResourceManager::createProgramFromFiles(std::string ref, std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
    ShaderProgram* program = new ShaderProgram(); checkError();
    mProgramCache.add(ref, program); checkError();

    Shader* vertexShader = createShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilename); checkError();
    Shader* fragmentShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename); checkError();

    program->setVertexShader(vertexShader); checkError();
    program->setFragmentShader(fragmentShader); checkError();

    std::string info = "";
    if(!program->link(&info))
    {
        std::cout << info << std::endl;
    }

    return program;
}

Material* ResourceManager::createMaterial(std::string ref)
{
    Material* material = new Material();
    mMaterialCache.add(ref, material);
    return material;
}

VertexArrayObject* ResourceManager::createVAO(std::string ref)
{
    VertexArrayObject* vao = new VertexArrayObject();
    mVaoCache.add(ref, vao);
    return vao;
}

Mesh* ResourceManager::createMesh(std::string ref)
{
    Mesh* mesh = new Mesh();
    mMeshCache.add(ref, mesh);
    return mesh;
}

Texture* ResourceManager::createTextureFromFile(std::string filename)
{
    Texture* texture = getTexture(filename);
    if(texture == 0)
    {
        texture = new Texture(GL_TEXTURE_2D);
        texture->init();
        texture->loadImage(filename);
        mTextureCache.add(filename, texture);
    }
    return texture;
}

Texture* ResourceManager::createTexture(std::string ref, GLenum target)
{
    Texture* texture = new Texture(target);
    mTextureCache.add(ref, texture);
    return texture;
}

Buffer* ResourceManager::createBuffer(GLenum bufferType, std::string ref)
{
    Buffer* buffer = new Buffer(bufferType);
    mBufferCache.add(ref, buffer);
    return buffer;
}

ArrayBuffer* ResourceManager::createArrayBuffer(GLenum bufferType, std::string ref)
{
    ArrayBuffer* arrayBuffer = new ArrayBuffer(bufferType);
    mArrayBufferCache.add(ref, arrayBuffer);
    return arrayBuffer;
}

UniformBuffer* ResourceManager::createUniformBuffer(std::string ref)
{
    UniformBuffer* uniformBuffer = new UniformBuffer();
    mUniformBufferCache.add(ref, uniformBuffer);
    return uniformBuffer;
}

Shader* ResourceManager::getShader(std::string ref)
{
    return mShaderCache.get(ref);
}

ShaderProgram* ResourceManager::getProgram(std::string ref)
{
    return mProgramCache.get(ref);
}

Material* ResourceManager::getMaterial(std::string ref)
{
    return mMaterialCache.get(ref);
}

VertexArrayObject* ResourceManager::getVAO(std::string ref)
{
    return mVaoCache.get(ref);
}

Mesh* ResourceManager::getMesh(std::string ref)
{
    return mMeshCache.get(ref);
}

Texture* ResourceManager::getTexture(std::string ref)
{
    return mTextureCache.get(ref);
}

Buffer* ResourceManager::getBuffer(std::string ref)
{
    return mBufferCache.get(ref);
}

ArrayBuffer* ResourceManager::getArrayBuffer(std::string ref)
{
    return mArrayBufferCache.get(ref);
}

UniformBuffer* ResourceManager::getUniformBuffer(std::string ref)
{
    return mUniformBufferCache.get(ref);
}

Mesh* ResourceManager::loadMeshFromObj(std::string filename, ShaderProgram* shaderProgram)
{
    checkError();
    Mesh* mesh = getMesh(filename);
    Material* material = 0;
    if(mesh != 0) return mesh;

    mesh = createMesh(filename);

    std::ifstream fileStream;
    fileStream.open(filename.c_str(), std::ios::binary);
checkError();
    if(fileStream.is_open())
    {
        char currentLine[200];

        //The final attributes pumped into the VAO.
        std::vector<GLuint> meshIndices;
        std::vector<GLfloat> meshCoords;
        std::vector<GLfloat> meshNormals;
        std::vector<GLfloat> meshTexCoords;

        //The temporary attributes that are retrieved from the .obj
        std::vector<GLfloat> objCoords;
        std::vector<GLfloat> objNormals;
        std::vector<GLfloat> objTexCoords;
checkError();
        while(fileStream.good())
        {
            fileStream.getline(currentLine, 200);

            char* firstToken = strtok(currentLine, " ");

            if(firstToken != 0)
            {
                if(strcmp(firstToken, "v") == 0)
                {
                    //Vertex
                    char* currCoordC = strtok(0, " ");
                    while(currCoordC)
                    {
                        float currCoordF = strtof(currCoordC, 0);
                        objCoords.push_back(currCoordF);
                        currCoordC = strtok(0, " ");
                    }checkError();
                }
                else if(strcmp(firstToken, "vt") == 0)
                {
                    //Tex Coord
                    char* currCoordC = strtok(0, " ");
                    while(currCoordC)
                    {
                        float currCoordF = strtof(currCoordC, 0);
                        objTexCoords.push_back(currCoordF);
                        currCoordC = strtok(0, " ");
                    }
                }
                else if(strcmp(firstToken, "vn") == 0)
                {
                    //Normal
                    char* currCoordC = strtok(0, " ");
                    while(currCoordC)
                    {
                        float currCoordF = strtof(currCoordC, 0);
                        objNormals.push_back(currCoordF);
                        currCoordC = strtok(0, " ");
                    }checkError();
                }
                else if(strcmp(firstToken, "f") == 0)
                {
                    //Face
                    char* currVert = strtok(0, " ");
                    char* verts[3];
                    int i = 0;

                    //Split the face into 3 verts, each is: coordIndex/texCoordIndex/normalIndex
                    while(currVert && i < 3)
                    {
                        verts[i] = currVert;
                        i++;
                        currVert = strtok(0, " ");
                    }

                    for(int i = 0; i < 3; i++)
                    {
                        char* coordIndexC = strtok(verts[i], "/");
                        char* texCoordIndexC = strtok(0, "/");
                        char* normalIndexC = strtok(0, "/");

                        int coordIndexI = (atoi(coordIndexC) - 1)*3;
                        int texCoordIndexI = (atoi(texCoordIndexC) - 1)*2;
                        int normalIndexI = (atoi(normalIndexC) - 1)*3;

                        int index = meshIndices.size();

                        meshIndices.push_back(index);

                        meshCoords.push_back(objCoords[coordIndexI]);
                        meshCoords.push_back(objCoords[coordIndexI+1]);
                        meshCoords.push_back(objCoords[coordIndexI+2]);

                        meshTexCoords.push_back(objTexCoords[texCoordIndexI]);
                        meshTexCoords.push_back(1.0f-objTexCoords[texCoordIndexI+1]);

                        meshNormals.push_back(objNormals[normalIndexI]);
                        meshNormals.push_back(objNormals[normalIndexI+1]);
                        meshNormals.push_back(objNormals[normalIndexI+2]);
                    }checkError();
                }
                else if(strcmp(firstToken, "mtllib") == 0)
                {
                    std::string materialFilename = strtok(0, " ");
                    material = loadMaterialFromMtl("material/" + materialFilename);checkError();
                }
                else if(strcmp(firstToken, "usemtl") == 0)
                {
                    char* materialName = strtok(0, " ");
                    mesh->setMaterial(material);checkError();
                }
            }

        }
checkError();
        //Create VAO
        unsigned int numFloats = (meshIndices.size()*(3+2+3)); //Per vertex, 3 coordinates, 2 tex coords, 3 normal coords.
        GLfloat* vertexAttribArray = new GLfloat[numFloats];

        for(unsigned int i = 0; i < meshIndices.size(); i++)
        {
            unsigned int vertexStart = i * (3+2+3);
            unsigned int coordStart = i * 3;
            unsigned int texStart = i * 2;
            unsigned int normalStart = i * 3;

            vertexAttribArray[vertexStart] = meshCoords[coordStart];
            vertexAttribArray[vertexStart+1] = meshCoords[coordStart+1];
            vertexAttribArray[vertexStart+2] = meshCoords[coordStart+2];

            vertexAttribArray[vertexStart+3] = meshTexCoords[texStart];
            vertexAttribArray[vertexStart+4] = meshTexCoords[texStart+1];

            vertexAttribArray[vertexStart+5] = meshCoords[normalStart];
            vertexAttribArray[vertexStart+6] = meshCoords[normalStart+1];
            vertexAttribArray[vertexStart+7] = meshCoords[normalStart+2];
        }

        GLuint positionLocation = shaderProgram->getAttribLocation("v3_position");
        GLuint texcoordLocation = shaderProgram->getAttribLocation("v2_texcoord");
        GLuint normalLocation = shaderProgram->getAttribLocation("v3_normal");

        ArrayBuffer* arrayBuffer = createArrayBuffer(GL_ARRAY_BUFFER, filename+"_array_buffer");
        arrayBuffer->init();
        arrayBuffer->bind();
        arrayBuffer->setData(vertexAttribArray, numFloats, sizeof(GLfloat));
        arrayBuffer->unbind();

        GLuint vertexArray = 0;
        GLuint textureArray = 1;
        GLuint normalArray = 2;
        arrayBuffer->setArray(vertexArray, 3, GL_FLOAT, 8*sizeof(GLfloat), 0, positionLocation);
        arrayBuffer->setArray(textureArray, 2, GL_FLOAT, 8*sizeof(GLfloat), 3*sizeof(GLfloat), texcoordLocation);
        arrayBuffer->setArray(normalArray, 3, GL_FLOAT, 8*sizeof(GLfloat), 5*sizeof(GLfloat), normalLocation);

        Buffer* indexBuffer = createBuffer(GL_ELEMENT_ARRAY_BUFFER, filename+"_index_buffer");
        indexBuffer->init();
        indexBuffer->bind();
        indexBuffer->setData(&meshIndices[0], meshIndices.size(), sizeof(GLuint));
        indexBuffer->unbind();

        VertexArrayObject* vao = createVAO(filename+"_vao");
        vao->init();
        vao->bind();
        vao->linkVertexArray(positionLocation, arrayBuffer, vertexArray);
        vao->linkVertexArray(texcoordLocation, arrayBuffer, textureArray);
        vao->linkVertexArray(normalLocation, arrayBuffer, normalArray);
        vao->unbind();

        delete[] vertexAttribArray;

        mesh->setVAO(vao);
        mesh->setIndexBuffer(indexBuffer);
        mesh->setNumIndices(meshIndices.size());
        mesh->setMaterial(material);checkError();
        mesh->setProgram(shaderProgram);
    }
    fileStream.close();checkError();
    return mesh;
}

Material* ResourceManager::loadMaterialFromMtl(std::string filename)
{
    Material* material = getMaterial(filename);

    if(material != 0) return material;
    material = createMaterial(filename);

    std::ifstream fileStream;
    fileStream.open(filename.c_str(), std::ios::binary);

    if(fileStream.is_open())
    {
        char currentLine[200];

        while(fileStream.good())
        {
            fileStream.getline(currentLine, 200);

            char* firstToken = strtok(currentLine, " ");

            if(firstToken != 0)
            {
                if(strcmp(firstToken, "newmtl") == 0)
                {
                    char* materialName = strtok(0, " ");
                    //material = createMaterial(materialName);
                }
                else if(strcmp(firstToken, "Ka") == 0)
                {
                    char* currColorVal = strtok(0, " ");
                    float colorVals[3];
                    unsigned int index = 0;
                    while(currColorVal)
                    {
                        float currColorValF = strtof(currColorVal, 0);
                        colorVals[index] = currColorValF;
                        index++;
                        currColorVal = strtok(0, " ");
                    }
                    material->setAmbient(colorVals[0], colorVals[1], colorVals[2], 1.0f);
                }
                else if(strcmp(firstToken, "Kd") == 0)
                {
                    char* currColorVal = strtok(0, " ");
                    float colorVals[3];
                    unsigned int index = 0;
                    while(currColorVal)
                    {
                        float currColorValF = strtof(currColorVal, 0);
                        colorVals[index] = currColorValF;
                        index++;
                        currColorVal = strtok(0, " ");
                    }
                    material->setDiffuse(colorVals[0], colorVals[1], colorVals[2], 1.0f);
                }
                else if(strcmp(firstToken, "Ks") == 0)
                {
                    char* currColorVal = strtok(0, " ");
                    float colorVals[3];
                    unsigned int index = 0;
                    while(currColorVal)
                    {
                        float currColorValF = strtof(currColorVal, 0);
                        colorVals[index] = currColorValF;
                        index++;
                        currColorVal = strtok(0, " ");
                    }
                    material->setSpecular(colorVals[0], colorVals[1], colorVals[2], 1.0f);
                }
                else if(strcmp(firstToken, "map_Kd") == 0)
                {
                    std::string textureFilename = strtok(0, " ");
                    Texture* texture = createTextureFromFile("texture/" + textureFilename);
                    texture->bind();
                    texture->generateMipMaps(1000);
                    texture->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                    texture->setMaxFilter(GL_LINEAR);
                    texture->setWrapS(GL_REPEAT);
                    texture->setWrapT(GL_REPEAT);
                    texture->unbind();
                     material->setTexture(texture);
                }
            }

        }
    }
    fileStream.close();
    return material;
}
