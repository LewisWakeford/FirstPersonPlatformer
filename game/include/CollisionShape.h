#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

enum CollisionShapeType
{
    SHAPE_INVALID,
    SHAPE_CUBOID,
    SHAPE_POINT,
    SHAPE_RAY
};

class BoundingBox
{
    public:
        static BoundingBox composite(const BoundingBox& A, const BoundingBox& B);
        static bool overlap(const BoundingBox& A, const BoundingBox& B);

        BoundingBox(float x, float y, float z, float xSize, float ySize, float zSize)
        {
            mX = x;
            mY = y;
            mZ = z;
            mXSize = xSize;
            mYSize = ySize;
            mZSize = zSize;
        };
        virtual ~BoundingBox(){};

        float mX;
        float mY;
        float mZ;
        float mXSize;
        float mYSize;
        float mZSize;
};

class CollisionShape
{
    public:
        CollisionShape();
        virtual ~CollisionShape();

        virtual BoundingBox getBoundingBox() const;

        const CollisionShapeType getType() const;


    protected:
        CollisionShapeType mType;

    private:
};

#endif // COLLISIONSHAPE_H
