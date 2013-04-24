#ifndef SCENENODE_H
#define SCENENODE_H

#include <vector>

#include "game.h"

#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>

#include "I_CollisionReciever.h"


class SceneNode;
class App;
typedef boost::shared_ptr<SceneNode> SceneNodePtr;

/*
Class: SceneNode
Description: SuperClass for all "Objects" in the game. Anything that requires per tick simulation and/or per frame drawing.
*/
class SceneNode : public I_CollisionReciever
{
    public:
        SceneNode(App* app, GLenum renderPass);
        virtual ~SceneNode();

        virtual void onEvent();     //Called whenever an event occurs that this node was listening for.
        virtual void onInit();      //Should be called before this node is used as part of the simulation.
        virtual void onLoop(GLdouble deltaTime);      //Perform per tick simulation.
        virtual void simulateSelf(GLdouble deltaTime);
        virtual void onRender(GLenum renderPass);    //Render the object to the vertex buffer.
        virtual void renderSelf();
        virtual void onCleanup();   //Perform before destroying the actual object.

        void setTransform(glm::mat4 transform);
        virtual glm::mat4 getTransform() const;
        void setAbsoluteTransform(glm::mat4 transform);

        void addChild(SceneNodePtr child);
        void addCollider(Collider* collider);

        void pushColliders();

        virtual void onCollision(CollisionEvent event);

        GLboolean isDeleted();
        void deleteNode();

    protected:
        GLenum mRenderPass;                     //The render pass to draw this node in.

        glm::mat4 mTransform;           //The 4x4 Matrix representing the transform of this node.
        glm::mat4 mAbsoluteTransform;           //The 4x4 Matrix representing the absolute transform of this node.
        SceneNodePtr mParent;                   //Pointer to the parent of this node.
        std::vector<SceneNodePtr> mChildList;   //A list of the children of this node.
        std::vector<Collider*> mColliderList;

        GLboolean mDeleted;

        App* mApp;

    private:
};

#endif // SCENENODE_H
