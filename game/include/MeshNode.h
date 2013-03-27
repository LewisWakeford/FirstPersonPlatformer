#ifndef MESHNODE_H
#define MESHNODE_H

#include "SceneNode.h"

class App;
class Mesh;

class MeshNode : public SceneNode
{
    public:
        MeshNode(App* app, GLenum renderPass);
        MeshNode(App* app, GLenum renderPass, Mesh* mesh);
        virtual ~MeshNode();

        void setMesh(Mesh* meshIndex);

       virtual void renderSelf();

    protected:

        //Pointer to mesh resource.
        //Cannot modify from here.
        const Mesh* mMesh;
    private:
};

#endif // MESHNODE_H
