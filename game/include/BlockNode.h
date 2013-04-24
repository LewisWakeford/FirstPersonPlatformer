#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <PersistantNode.h>

class Mesh;

class BlockNode : public PersistantNode
{
    public:
        BlockNode(App* app);
        virtual ~BlockNode();

        void setOrigin(std::vector<float> origin);
        void setXSize(int negX, int posX);
        void setYSize(int negY, int posY);
        void setZSize(int negZ, int posZ);

        void setNorthFace(std::vector<int> northFace);
        void setEastFace(std::vector<int> eastFace);
        void setSouthFace(std::vector<int> southFace);
        void setWestFace(std::vector<int> westFace);

        void setRotation(float rotation);

        std::vector<float> getOrigin();
        int getNegX();
        int getPosX();
        int getNegY();
        int getPosY();
        int getNegZ();
        int getPosZ();

        std::vector<int> getNorthFace();
        std::vector<int> getEastFace();
        std::vector<int> getSouthFace();
        std::vector<int> getWestFace();

        bool isClimable(unsigned int face, unsigned int u, unsigned int v);
        bool isClimable(glm::vec3 point);

        float getRotation();

        void simulateSelf(GLdouble deltaTime);
        void renderSelf();

    protected:
        std::vector<float> mOrigin; //Change to glm vector.

        int mNegX;
        int mPosX;
        int mNegY;
        int mPosY;
        int mNegZ;
        int mPosZ;

        std::vector<int> mNorthFace;
        std::vector<int> mEastFace;
        std::vector<int> mSouthFace;
        std::vector<int> mWestFace;

        float mRotation;

        //Mesh Object
        Mesh* mMesh;
        bool mMeshBuilt;

        void buildMesh();
        void buildFace(std::vector<GLfloat>& vertexArray, std::vector<GLuint>& indexArray,
                        unsigned int face,
                        glm::vec3 start,
                        unsigned int upSize,
                        unsigned int sideSize,
                        glm::vec3 upwards,
                        glm::vec3 sideways);

        unsigned int mBlockId;

        static unsigned int gBlockId;
        static unsigned int getNewID();

    private:
};

#endif // BLOCKNODE_H
