#include "BlockNode.h"

#include "App.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Block.h"
#include "CuboidShape.h"
#include "Orientation.h"
#include "Collider.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int BlockNode::gBlockId = 0;

unsigned int BlockNode::getNewID()
{
    return BlockNode::gBlockId;
    BlockNode::gBlockId++;
}

BlockNode::BlockNode(App* app) : PersistantNode(app, GAME_RENDER_GEOMETRY)
{
    mMesh = 0;
    mMeshBuilt = false;
    mBlockId = getNewID();
}

BlockNode::~BlockNode()
{
    //dtor
}

void BlockNode::setOrigin(std::vector<float> origin)
{
    mOrigin = origin;
}

void BlockNode::setXSize(int negX, int posX)
{
    mNegX = negX;
    mPosX = posX;
}

void BlockNode::setYSize(int negY, int posY)
{
    mNegY = negY;
    mPosY = posY;
}

void BlockNode::setZSize(int negZ, int posZ)
{
    mNegZ = negZ;
    mPosZ = posZ;
}

void BlockNode::setNorthFace(std::vector<int> northFace)
{
    mNorthFace = northFace;
}

void BlockNode::setEastFace(std::vector<int> eastFace)
{
    mEastFace = eastFace;
}

void BlockNode::setSouthFace(std::vector<int> southFace)
{
    mSouthFace = southFace;
}

void BlockNode::setWestFace(std::vector<int> westFace)
{
    mWestFace = westFace;
}

void BlockNode::setRotation(float rotation)
{
    mRotation = rotation;
}

std::vector<float> BlockNode::getOrigin()
{
    return mOrigin;
}

int BlockNode::getNegX()
{
    return mNegX;
}

int BlockNode::getPosX()
{
    return mPosX;
}

int BlockNode::getNegY()
{
    return mNegY;
}

int BlockNode::getPosY()
{
    return mPosY;
}

int BlockNode::getNegZ()
{
    return mNegZ;
}

int BlockNode::getPosZ()
{
    return mPosZ;
}

std::vector<int> BlockNode::getNorthFace()
{
    return mNorthFace;
}

std::vector<int> BlockNode::getEastFace()
{
    return mEastFace;
}

std::vector<int> BlockNode::getSouthFace()
{
    return mSouthFace;
}

std::vector<int> BlockNode::getWestFace()
{
    return mWestFace;
}

float BlockNode::getRotation()
{
    return mRotation;
}

void BlockNode::simulateSelf(GLdouble deltaTime)
{
    if(!mMeshBuilt)
    {
        buildMesh();
    }
}

void BlockNode::renderSelf()
{
    glm::mat4 MVP = mApp->getRenderer()->getProjectionMatrix() * mApp->getRenderer()->getViewMatrix() * mApp->getRenderer()->currentMatrix();
    mMesh->render(MVP);
}

void BlockNode::buildMesh()
{
    checkError();

    mTransform = glm::translate(glm::mat4(1.0f), glm::vec3(float(mOrigin[0]), float(mOrigin[1]), float(mOrigin[2])));

    //Create a new mesh.
    std::string ref = "block_" + mBlockId;
    mMesh = mApp->getResourceManager()->createMesh(ref);

    GLfloat panelSize = 1.0f;

    //Calculate the required number of vertices and indices:
    unsigned int width = mPosX + mNegX + 1;
    unsigned int depth = mPosZ + mNegZ + 1;
    unsigned int height = mPosY + mNegY + 1;

    unsigned int numberOfPanels = ((width * depth) + (depth * height) + (width * height)) * 2;

    unsigned int numVertices = numberOfPanels * 4; //Don't think I can share them as tex coords change.
    unsigned int numIndices = numberOfPanels * 6;

    std::vector<GLfloat> blockVertices;
    blockVertices.resize((3*numVertices) + (2*numVertices));//3 floats per vertex for coords, 2 floats per vertex for texcoords.

    std::vector<GLuint> blockIndices;
    blockIndices.resize(numIndices);

    GLfloat fPosX = GLfloat(mPosX) + (0.5f * panelSize);
    GLfloat fNegX = GLfloat(-mNegX) - (0.5f * panelSize);
    GLfloat fPosY = GLfloat(mPosY) + (0.5f * panelSize);
    GLfloat fNegY = GLfloat(-mNegY) - (0.5f * panelSize);
    GLfloat fPosZ = GLfloat(mPosZ) + (0.5f * panelSize);
    GLfloat fNegZ = GLfloat(-mNegZ) - (0.5f * panelSize);

    //Y is up, -Z is forward, X is right

    //Generate Pos X face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_EAST,
                            glm::vec3(fPosX, fNegY, fNegZ), //Starting point for vertices.
                            height, depth,                   //Amount to extend up and sideways.
                            glm::vec3(0.0f, 1.0f, 0.0f),     //Which direction is up.
                            glm::vec3(0.0f, 0.0f, 1.0f)      //Which direction is sideways.
              );

    //Genreate Neg X face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_WEST,
                            glm::vec3(fNegX, fNegY, fPosZ), //Starting point for vertices.
                            height, depth,                  //Amount to extend up and sideways.
                            glm::vec3(0.0f, 1.0f, 0.0f),     //Which direction is up.
                            glm::vec3(0.0f, 0.0f, -1.0f)      //Which direction is sideways.
              );

    //Generate Pos Z face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_NORTH,
                            glm::vec3(fPosX, fNegY, fPosZ), //Starting point for vertices.
                            height, width,                  //Amount to extend up and sideways.
                            glm::vec3(0.0f, 1.0f, 0.0f),     //Which direction is up.
                            glm::vec3(-1.0f, 0.0f, 0.0f)      //Which direction is sideways.
              );

    //Genreate Neg Z face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_SOUTH,
                            glm::vec3(fNegX, fNegY, fNegZ), //Starting point for vertices.
                            height, width,                  //Amount to extend up and sideways.
                            glm::vec3(0.0f, 1.0f, 0.0f),     //Which direction is up.
                            glm::vec3(1.0f, 0.0f, 0.0f)      //Which direction is sideways.
              );

    //Generate Pos Y face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_UP,
                            glm::vec3(fNegX, fPosY, fNegZ), //Starting point for vertices.
                            depth, width,                   //Amount to extend up and sideways.
                            glm::vec3(0.0f, 0.0f, 1.0f),     //Which direction is up.
                            glm::vec3(1.0f, 0.0f, 0.0f)      //Which direction is sideways.
              );

    //Genreate Neg Y face
    buildFace(blockVertices, blockIndices,
                            BLOCK_FACE_DOWN,
                            glm::vec3(fNegX, fNegY, fPosZ), //Starting point for vertices.
                            depth, width,                   //Amount to extend up and sideways.
                            glm::vec3(0.0f, 0.0f, -1.0f),     //Which direction is up.
                            glm::vec3(1.0f, 0.0f, 0.0f)      //Which direction is sideways.
              );

    //Get Vertex Attrib Locations

    //Create VBO
    VertexArrayObject* vao = mApp->getResourceManager()->createVAO(ref);checkError();
    vao->init();checkError();

    //Create Shader Program
    ShaderProgram* program = mApp->getResourceManager()->getProgram("block");checkError();
    if(program == 0)
    {
        program = mApp->getResourceManager()->createProgramFromFiles("block", "shader/block.vert", "shader/block.frag");checkError();
    }

    GLuint positionLocation = program->getAttribLocation("v3_position");checkError();
    GLuint texcoordLocation = program->getAttribLocation("v2_texcoord");checkError();

    //Build Vertex Buffer
    ArrayBuffer* vertexBuffer = mApp->getResourceManager()->createArrayBuffer(GL_ARRAY_BUFFER, ref + "_vertex_buffer");checkError();
    vertexBuffer->init();
    vertexBuffer->bind();
    vertexBuffer->setData(&blockVertices[0], blockVertices.size(), sizeof(GLfloat));
    vertexBuffer->unbind();
    GLuint vertexArray = 0;
    GLuint textureArray = 1;
    vertexBuffer->setArray(vertexArray, 3, GL_FLOAT, 5*sizeof(GLfloat), 0, positionLocation);checkError();
    vertexBuffer->setArray(textureArray, 2, GL_FLOAT, 5*sizeof(GLfloat), 3*sizeof(GLfloat), texcoordLocation);checkError();

    vao->bind();
    vao->linkVertexArray(positionLocation, vertexBuffer, vertexArray);checkError();
    vao->linkVertexArray(texcoordLocation, vertexBuffer, textureArray);checkError();
    vao->unbind();


    //Build Index Buffer
    Buffer* indexBuffer = mApp->getResourceManager()->createBuffer(GL_ELEMENT_ARRAY_BUFFER, ref + "_index_buffer");
    indexBuffer->init();
    indexBuffer->bind();
    indexBuffer->setData(&blockIndices[0], blockIndices.size(), sizeof(GLuint));
    indexBuffer->unbind();checkError();


    //Create Texture
    Texture* texture = mApp->getResourceManager()->getTexture("texture/panels.png");checkError();
    if(texture == 0)
    {
        texture = mApp->getResourceManager()->createTextureFromFile("texture/panels.png");checkError();
        texture->bind();
        texture->generateMipMaps(1000);
        texture->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        texture->setMaxFilter(GL_LINEAR);
        texture->setWrapS(GL_REPEAT);
        texture->setWrapT(GL_REPEAT);
        texture->unbind();
    }

    //Create Material
    Material* material = mApp->getResourceManager()->getMaterial("block");checkError();
    if(material == 0)
    {
        material = mApp->getResourceManager()->createMaterial("block");checkError();
        material->setTexture(texture);
    }

    //Build Uniform Buffer

    mMesh->setVAO(vao);
    mMesh->setIndexBuffer(indexBuffer);
    mMesh->setNumIndices(blockIndices.size());
    mMesh->setProgram(program);
    mMesh->setMaterial(material);

    mMeshBuilt = true;

    //Build Collision Shape
    CuboidShape* shape = new CuboidShape(glm::vec3(fNegX, fNegY, fNegZ), float(width), float(height), float(depth));
    addCollider(new Collider(shape, 0, this, GAME_COLLISION_BLOCK, GAME_COLLISION_NONE));
}

void BlockNode::buildFace(std::vector<GLfloat>& vertexArray, std::vector<GLuint>& indexArray,
                        unsigned int face,
                        glm::vec3 start,
                        unsigned int upSize,
                        unsigned int sideSize,
                        glm::vec3 upwards,
                        glm::vec3 sideways)
{
    GLfloat panelSize = 1.0f;

    for(unsigned int v = 0; v < upSize; v++)
    {
         for(unsigned int u = 0; u < sideSize; u++)
        {
            GLuint indexOffset = vertexArray.size()/5;

            //Bottom Left of the Panel, assuming it is facing us.
            glm::vec3 bottomLeft = start;

            glm::vec2 bottomLeftTex(0.0f, 0.0f);

            if(isClimable(face, u, v))
            {
                bottomLeftTex = glm::vec2(0.5f, 0.0f);
            }

            bottomLeft += (sideways * float(u)) * panelSize;
            bottomLeft += (upwards * float(v)) * panelSize;

            //Top Left of the Panel
            glm::vec3 topLeft = bottomLeft;
            glm::vec2 topLeftTex = bottomLeftTex + glm::vec2(0.0f, 1.0f);
            topLeft += upwards * panelSize;

            //Bottom Right of the Panel
            glm::vec3 bottomRight = bottomLeft;
            glm::vec2 bottomRightTex = bottomLeftTex + glm::vec2(0.5f, 0.0f);
            bottomRight += sideways * panelSize;

            //Top Right of the Panel
            glm::vec3 topRight = topLeft;
            glm::vec2 topRightTex = bottomLeftTex + glm::vec2(0.5f, 1.0f);
            topRight += sideways * panelSize;

            //Bottom Left = 0
            vertexArray.push_back(bottomLeft.x); vertexArray.push_back(bottomLeft.y); vertexArray.push_back(bottomLeft.z);
            vertexArray.push_back(bottomLeftTex.x); vertexArray.push_back(bottomLeftTex.y);

            //Top Left = 1
            vertexArray.push_back(topLeft.x); vertexArray.push_back(topLeft.y); vertexArray.push_back(topLeft.z);
            vertexArray.push_back(topLeftTex.x); vertexArray.push_back(topLeftTex.y);

            //Bottom Right = 2
            vertexArray.push_back(bottomRight.x); vertexArray.push_back(bottomRight.y); vertexArray.push_back(bottomRight.z);
            vertexArray.push_back(bottomRightTex.x); vertexArray.push_back(bottomRightTex.y);

            //Top Right = 3
            vertexArray.push_back(topRight.x); vertexArray.push_back(topRight.y); vertexArray.push_back(topRight.z);
            vertexArray.push_back(topRightTex.x); vertexArray.push_back(topRightTex.y);

            indexArray.push_back(indexOffset);
            indexArray.push_back(indexOffset+1);
            indexArray.push_back(indexOffset+2);

            indexArray.push_back(indexOffset+2);
            indexArray.push_back(indexOffset+1);
            indexArray.push_back(indexOffset+3);
        }
    }
}


bool BlockNode::isClimable(unsigned int face, unsigned int u, unsigned int v)
{
    std::vector<int>* theFace;

    if(face == BLOCK_FACE_NORTH)
        theFace = &mNorthFace;

    else if(face == BLOCK_FACE_EAST)
        theFace = &mEastFace;

    else if(face == BLOCK_FACE_SOUTH)
        theFace = &mSouthFace;

    else if(face == BLOCK_FACE_WEST)
        theFace = &mWestFace;

    else return false;

    for(unsigned int i = 0; i < theFace->size(); i+=2)
    {
        unsigned int faceU = theFace->at(i);
        unsigned int faceV = theFace->at(i+1);
        if(u == faceU && v == faceV)
        {
            return true;
        }
    }

    return false;
}
