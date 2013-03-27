#include "MeshNode.h"
#include "App.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Mesh.h"

MeshNode::MeshNode(App* app, GLenum renderPass) : SceneNode(app, renderPass)
{
    //ctor
}

MeshNode::MeshNode(App* app, GLenum renderPass, Mesh* mesh) : SceneNode(app, renderPass)
{
    mMesh = mesh;
}

MeshNode::~MeshNode()
{
    //dtor
}

void MeshNode::setMesh(Mesh* mesh)
{
    mMesh = mesh;
}

void MeshNode::renderSelf()
{
    //Render mesh
    glm::mat4 MVP = mApp->getRenderer()->getProjectionMatrix() * mApp->getRenderer()->getViewMatrix() * mApp->getRenderer()->currentMatrix();
    //glm::mat4 MVP = mApp->getRenderer()->getProjectionMatrix() * mApp->getRenderer()->currentMatrix();
    if(mMesh != -0)
        mMesh->render(MVP);
}
