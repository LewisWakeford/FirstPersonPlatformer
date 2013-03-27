#ifndef PERSISTANTNODE_H
#define PERSISTANTNODE_H

#include <SceneNode.h>


class PersistantNode : public SceneNode
{
    public:
        PersistantNode(App* app, GLenum renderPass);
        virtual ~PersistantNode();

        bool hasChanged();

    protected:
        bool mHasChanged;

    private:
};

#endif // PERSISTANTNODE_H
