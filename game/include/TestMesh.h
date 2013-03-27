#ifndef TESTMESH_H
#define TESTMESH_H

#include <MeshNode.h>

/*
    Class: Test Mesh
    A really simple subclass of MeshNode designed to test the renderer is worknig.
*/
class TestMesh : public MeshNode
{
    public:
        TestMesh(App* app);
        virtual ~TestMesh();
    protected:
    private:
};

#endif // TESTMESH_H
