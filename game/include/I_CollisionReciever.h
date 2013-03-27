#ifndef I_COLLISIONRECIEVER_H
#define I_COLLISIONRECIEVER_H

#include "CollisionEvent.h"

class I_CollisionReciever
{
    public:
        virtual void onCollision(CollisionEvent event) = 0;

    protected:
        I_CollisionReciever();
        virtual ~I_CollisionReciever();
    private:
};

#endif // I_COLLISIONRECIEVER_H
