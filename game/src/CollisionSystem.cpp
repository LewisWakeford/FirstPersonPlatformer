#include "CollisionSystem.h"
#include "CuboidShape.h"
#include "RayShape.h"

CollisionSystem::CollisionSystem()
{

}

CollisionSystem::~CollisionSystem()
{

}

void CollisionSystem::pushCollider(Collider* collider)
{
    collider->transformBy(currentMatrix());
    mObjectList.push_back(collider);
}

void CollisionSystem::checkCollisions()
{
    //If collision checking starts to get slow, maybe add a broadER phase with sheres?

    //Broadphase Collision, find pairs that potentially collide.
    std::vector<Collider*> collisionPairs;
    for(unsigned int i = 0; i < mObjectList.size(); i++)
    {
        Collider* A = mObjectList[i];
        for(unsigned int k = i+1; k < mObjectList.size(); k++)
        {
            Collider* B = mObjectList[k];
            bool ACollides = A->collidesWith(B->getGroup());
            bool BCollides = B->collidesWith(A->getGroup());
            if(ACollides || BCollides)
            {
                bool broadphaseResult = broadphaseTest(A, B);
                if(broadphaseResult)
                {
                    collisionPairs.push_back(A);
                    collisionPairs.push_back(B);
                }
            }
        }
    }

    //NarrowPhase, more precise collision checking.
    for(unsigned int i = 0; i < collisionPairs.size(); i+=2)
    {
        Collider* A = collisionPairs[i];
        Collider* B = collisionPairs[i+1];
        std::vector<glm::vec3> contactPoints;
        double contactTime;

        bool ACollides = A->collidesWith(B->getGroup());
        bool BCollides = B->collidesWith(A->getGroup());

        if(narrowphaseTest(A, B, &contactPoints, &contactTime))
        {
            if(ACollides)
            {
                CollisionEvent event(A, B, contactPoints, contactTime);
                A->addEvent(event);
            }
            if(BCollides)
            {
                CollisionEvent event(B, A, contactPoints, contactTime);
                B->addEvent(event);
            }
        }
    }

    for(unsigned int i = 0; i < mObjectList.size(); i++)
    {
        mObjectList[i]->processCollisions();
    }

    mObjectList.clear();
}

void CollisionSystem::pushMatrix(glm::mat4 matrix)
{
    if(mMatrixStack.size() > 0)
    {
        glm::mat4 temp = mMatrixStack.back();
        temp = temp * matrix;
        mMatrixStack.push_back(temp);
    }
    else
    {
        glm::mat4 temp = matrix;
        mMatrixStack.push_back(temp);
    }
}

glm::mat4 CollisionSystem::popMatrix()
{
    glm::mat4 temp = mMatrixStack.back();
    mMatrixStack.pop_back();
    return temp;
}

glm::mat4 CollisionSystem::currentMatrix()
{
    glm::mat4 temp = mMatrixStack.back();
    return temp;
}

bool CollisionSystem::broadphaseTest(const Collider* A, const Collider* B)
{
    BoundingBox prevBoundingBoxA = A->getBoundingBox(0.0);
    BoundingBox currBoundingBoxA = A->getBoundingBox(1.0);
    BoundingBox compositeBoundingBoxA = BoundingBox::composite(prevBoundingBoxA, currBoundingBoxA);

    BoundingBox prevBoundingBoxB = B->getBoundingBox(0.0);
    BoundingBox currBoundingBoxB = B->getBoundingBox(1.0);
    BoundingBox compositeBoundingBoxB = BoundingBox::composite(prevBoundingBoxB, currBoundingBoxB);

    return BoundingBox::overlap(compositeBoundingBoxA, compositeBoundingBoxB);
}

bool CollisionSystem::narrowphaseTest(const Collider* A, const Collider* B, std::vector<glm::vec3>* contactPoints, double* contactTime)
{
    CollisionShapeType typeA = A->getShape()->getType();
    CollisionShapeType typeB = B->getShape()->getType();

    if(typeA == SHAPE_CUBOID && typeB == SHAPE_CUBOID)
    {
        return narrowCuboidOnCuboid(A, B, contactPoints, contactTime);
    }
    if(typeA == SHAPE_RAY && typeB == SHAPE_CUBOID)
    {
        return narrowRayOnCuboid(A, B, contactPoints, contactTime);
    }
    if(typeB == SHAPE_RAY && typeA == SHAPE_CUBOID)
    {
        return narrowRayOnCuboid(B, A, contactPoints, contactTime);
    }
    else return false;
}

bool CollisionSystem::narrowCuboidOnCuboid(const Collider* A, const Collider* B, std::vector<glm::vec3>* contactPoints, double* contactTime)
{
    CuboidShape* shapeA = (CuboidShape*) A->getShape();
    CuboidShape* shapeB = (CuboidShape*) B->getShape();

    //Bounding Box Collision would not have passed if they did not intersect in Y, and as I only ever yaw the shapes the bounding box collision in Y is 1:1.

    //Find the point in Y where intersection may occur.

    //TODO: check they are not overlapping at start of step.

    //Determine in which direction in Y the shapes are moving.
    Orientation startA = A->getOrientation(0.0);
    Orientation endA = A->getOrientation(1.0);

    Orientation startB = B->getOrientation(0.0);
    Orientation endB = B->getOrientation(1.0);

    float yStartA = startA.getPos().y;
    float yStartB = startB.getPos().y;

    float yMovementA = yStartA - endA.getPos().y; //Negative = moving up, pos = moving down.
    float yMovementB = yStartB - endB.getPos().y;

    //If they are moveing in opposite directions, intersection is not possible:
    if( (yMovementA < 0 && yMovementB > 0) || (yMovementA > 0 && yMovementB < 0) )
    {
        return false;
    }

    float ySpeedA = yMovementA;
    float ySpeedB = yMovementB;

    int yDir = 0;

    if(yMovementA != 0)
    {
        if(yMovementA > 0)
        {
            yDir = -1;
        }
        else
        {
            yDir = 1;
        }
    }
    else if(yMovementB != 0)
    {
        if(yMovementB > 0)
        {
            yDir = -1;
        }
        else
        {
            yDir = 1;
        }
    }

    //Use the movement direction and the height of the objects to determine which edges to test with.

    float yStartEdgeA;
    float yStartEdgeB;

    glm::mat4 matrixStartA = startA.getOrientationMatrix();
    glm::mat4 matrixStartB = startB.getOrientationMatrix();

    bool alreadyCollided = false;

    if(yStartA > yStartB) //A higher
    {
        yStartEdgeA = (matrixStartA * glm::vec4(shapeA->getPos(), 1.0f)).y; //Bottom Edge
        yStartEdgeB = (matrixStartB * glm::vec4(shapeB->getPos(), 1.0f)).y+shapeB->getYSize(); //Top Edge

        if(yStartEdgeA < yStartEdgeB)
        {
            //Already collided.
            alreadyCollided = true;
        }

    }
    else //A lower
    {
        yStartEdgeA = (matrixStartA * glm::vec4(shapeA->getPos(), 1.0f)).y+shapeA->getYSize(); //Top Edge
        yStartEdgeB = (matrixStartB * glm::vec4(shapeB->getPos(), 1.0f)).y;//Bottom Edge

        if(yStartEdgeB < yStartEdgeA)
        {
            //Already collided.
            alreadyCollided = true;
        }
    }

    //The equation for finding when the shapes overlap is: startA + (speedA * time) = startB + (speedB * time) => time = (startA - startB)/(speedB - speedA)
    //If time returns as < 0 or a divide by zero occurs, then they do not overlap in Y

    if(ySpeedB == ySpeedA)
    {
        return false;
    }

    double timeOfYIntersection = 0.0;
    if(!alreadyCollided)
    {
        timeOfYIntersection = (yStartEdgeA - yStartEdgeB)/(ySpeedA - ySpeedB);
    }
    float pointOfYIntersection = yStartEdgeA + (timeOfYIntersection * ySpeedA);

    if(timeOfYIntersection < 0 || timeOfYIntersection > 1)
    {
        return false;
    }

    //Get orientation at timestep
    Orientation orientAtTimeA = A->getOrientation(timeOfYIntersection);
    Orientation orientAtTimeB = B->getOrientation(timeOfYIntersection);

    //Get 2D shapes at timestep
    glm::mat4 matrixA = orientAtTimeA.getOrientationMatrix();
    glm::mat4 matrixB = orientAtTimeB.getOrientationMatrix();

    glm::vec3 cornersA[4];
    cornersA[0] = shapeA->getPos(); //Origin
    cornersA[1] = cornersA[0] + glm::vec3(shapeA->getXSize(), 0.0f, 0.0f);
    cornersA[2] = cornersA[1] + glm::vec3(0.0f, 0.0f, shapeA->getZSize());
    cornersA[3] = cornersA[0] + glm::vec3(0.0f, 0.0f, shapeA->getZSize());

    glm::vec3 cornersB[4];
    cornersB[0] = shapeB->getPos(); //Origin
    cornersB[1] = cornersB[0] + glm::vec3(shapeB->getXSize(), 0.0f, 0.0f);
    cornersB[2] = cornersB[1] + glm::vec3(0.0f, 0.0f, shapeB->getZSize());
    cornersB[3] = cornersB[0] + glm::vec3(0.0f, 0.0f, shapeB->getZSize());

    glm::vec2 cornersA_2D[4];
    glm::vec2 cornersB_2D[4];

    float minAX;
    float maxAX;
    float minAZ;
    float maxAZ;

    float minBX;
    float maxBX;
    float minBZ;
    float maxBZ;

    //Build AABBs from transformed BBs
    for(unsigned int i = 0; i < 4; i++)
    {
        glm::vec4 translatedA = (matrixA * glm::vec4(cornersA[i], 1.0f));
        cornersA_2D[i] = glm::vec2(translatedA.x, translatedA.z);

        glm::vec4 translatedB = (matrixB * glm::vec4(cornersB[i], 1.0f));
        cornersB_2D[i] = glm::vec2(translatedB.x, translatedB.z);

        if(i == 0)
        {
            minAX = translatedA.x;
            maxAX = translatedA.x;
            minAZ = translatedA.z;
            maxAZ = translatedA.z;

            minBX = translatedB.x;
            maxBX = translatedB.x;
            minBZ = translatedB.z;
            maxBZ = translatedB.z;
        }
        else
        {
            if(minAX > translatedA.x) minAX = translatedA.x;
            if(maxAX < translatedA.x) maxAX = translatedA.x;
            if(minAZ > translatedA.z) minAZ = translatedA.z;
            if(maxAZ < translatedA.z) maxAZ = translatedA.z;

            if(minBX > translatedB.x) minBX = translatedB.x;
            if(maxBX < translatedB.x) maxBX = translatedB.x;
            if(minBZ > translatedB.z) minBZ = translatedB.z;
            if(maxBZ < translatedB.z) maxBZ = translatedB.z;
        }
    }

    bool collision = false;

    //Check for complete encapsulation:
    bool encapsulated = true; //Is encapsulation possible?
    if(shapeA->getXSize() >= shapeB->getXSize() && shapeA->getZSize() >= shapeB->getZSize()) //Shape A is bigger
    {
        for(unsigned int i = 0; i < 4 && encapsulated; i++)
        {
            if(cornersB_2D[i].x < minAX || cornersB_2D[i].x > maxAX || cornersB_2D[i].y < minAZ || cornersB_2D[i].y > maxAZ) encapsulated = false;
        }
    }
    else if(shapeB->getXSize() > shapeA->getXSize() && shapeB->getZSize() > shapeA->getZSize()) //Shape B is bigger
    {
        for(unsigned int i = 0; i < 4 && encapsulated; i++)
        {
            if(cornersA_2D[i].x < minBX || cornersA_2D[i].x > maxBX || cornersA_2D[i].y < minBZ || cornersA_2D[i].y > maxBZ) encapsulated = false;
        }
    }

    for(unsigned int i = 0; i < 5; i++)
    {
        int p = (i + 1) % 4;
        glm::vec2 A1 = cornersA_2D[i];
        glm::vec2 A2 = cornersA_2D[p];
        for(unsigned int j = 0; j < 4; j++)
        {
            int q = (j + 1) % 4;
            glm::vec2 B1 = cornersB_2D[j];
            glm::vec2 B2 = cornersB_2D[q];
            glm::vec2 contactPoint(0.0f);
            if(lineIntersectionInBounds(A1, A2, B1, B2, &contactPoint))
            {
                contactPoints->push_back(glm::vec3(contactPoint.x, pointOfYIntersection, contactPoint.y));
                collision = true;
            }
        }
    }

    //If encapsulation was possible but no intersection then we know one shape is entirely within another
    if(encapsulated && !collision)
    {
        contactPoints->push_back(glm::vec3((minAX+maxAX/2), pointOfYIntersection, (minAZ+maxAZ)/2));
        collision = true;
    }

    if(collision)
    {
        *contactTime = timeOfYIntersection;
        return collision;
    }

    //Collision did not occur at the point of y intersection, however it might happen later.
    unsigned int closestPointA; //Point on shape A closest to shape B.
    unsigned int closestPointB; //Point on shape B closest to shape A.
    float distanceClosestA = -1.0f;
    float distanceClosestB = -1.0f;

    glm::vec2 centerOfA(0.0f); //The actual center of point A
    glm::vec2 centerOfB(0.0f); //The actual center of shape B

    for(unsigned int i = 0; i < 4; i++)
    {
        centerOfA += cornersA_2D[i];
        centerOfB += cornersB_2D[i];
    }

    centerOfA = centerOfA * 0.25f;
    centerOfB = centerOfB * 0.25f;

    for(unsigned int i = 0; i < 4; i++)
    {
        float distanceA = glm::distance(centerOfB, cornersA_2D[i]);
        float distanceB = glm::distance(centerOfA, cornersB_2D[i]);
        if(distanceClosestA < 0.0f || distanceClosestA > distanceA)
        {
            distanceClosestA = distanceA;
            closestPointA = i;
        }
        if(distanceClosestB < 0.0f || distanceClosestB > distanceB)
        {
            distanceClosestB = distanceB;
            closestPointB = i;
        }
    }

    //We have the closest points on each shape, if a collision happens on each time step it will occur between the edges of these points.
    //Only if one of the shapes is not moving will this be correct, but for now that's okay.

    glm::vec2* pointsMoving;
    unsigned int pointIndexMoving;
    glm::vec2* pointsStatic;
    unsigned int pointIndexStatic;
    glm::vec2 direction;

    float speedA = glm::distance(endA.getPos(), startA.getPos());
    float speedB = glm::distance(endB.getPos(), startA.getPos());

    if(speedA > speedB)
    {
        pointsMoving = cornersA_2D;
        pointIndexMoving = closestPointA;
        pointsStatic = cornersB_2D;
        pointIndexStatic = closestPointB;
        glm::vec3 direction3D = endA.getPos() - orientAtTimeA.getPos();
        direction = glm::vec2(direction3D.x, direction3D.z);
    }
    else
    {
        pointsMoving = cornersB_2D;
        pointIndexMoving = closestPointB;
        pointsStatic = cornersA_2D;
        pointIndexStatic = closestPointA;
        glm::vec3 direction3D = endB.getPos() - orientAtTimeB.getPos();
        direction = glm::vec2(direction3D.x, direction3D.z);
    }

    glm::vec2 pointsToTest[4*4];
    pointsToTest[0] = pointsMoving[pointIndexMoving];
    pointsToTest[1] = pointsMoving[pointIndexMoving] + direction;
    pointsToTest[2] = pointsStatic[pointIndexStatic];
    pointsToTest[3] = pointsStatic[pointIndexStatic+1%4];

    pointsToTest[4] = pointsMoving[pointIndexMoving];
    pointsToTest[5] = pointsMoving[pointIndexMoving] + direction;
    pointsToTest[6] = pointsStatic[pointIndexStatic];
    pointsToTest[7] = pointsStatic[pointIndexStatic-1%4];

    pointsToTest[8] = pointsStatic[pointIndexStatic];
    pointsToTest[9] = pointsStatic[pointIndexStatic] - direction;
    pointsToTest[10] = pointsMoving[pointIndexMoving];
    pointsToTest[11] = pointsMoving[pointIndexMoving+1%4];

    pointsToTest[12] = pointsStatic[pointIndexStatic];
    pointsToTest[13] = pointsStatic[pointIndexStatic] - direction;
    pointsToTest[14] = pointsMoving[pointIndexMoving];
    pointsToTest[15] = pointsMoving[pointIndexMoving-1%4];

    std::vector<double> contactTimes;
    std::vector<glm::vec3> tempContactPoints;

    for(unsigned int i = 0; i < 16; i+=4)
    {
        glm::vec2 pointOfIntersection(0.0f);
        if(lineIntersectionInBounds(pointsToTest[0], pointsToTest[1], pointsToTest[2], pointsToTest[3], &pointOfIntersection))
        {
            if(i < 8)
            {
                tempContactPoints.push_back(glm::vec3(pointOfIntersection.x, pointOfYIntersection, pointOfIntersection.y));
                contactTimes.push_back(glm::distance(pointOfIntersection, pointsToTest[0])/glm::length(direction));
                collision = true;
            }
            else
            {
                tempContactPoints.push_back(glm::vec3(pointsToTest[0].x, pointOfYIntersection, pointsToTest[0].y));
                contactTimes.push_back(glm::distance(pointOfIntersection, pointsToTest[0])/glm::length(direction));
                collision = true;
            }
        }
    }

    if(collision)
    {
        unsigned int shortestTimeIndex = 0;
        double shortestTime = contactTimes[shortestTimeIndex];

        for(int i = 1; i < contactTimes.size(); i++)
        {
            if(shortestTime > contactTimes[i])
            {
                shortestTime = contactTimes[i];
                shortestTimeIndex = i;
            }
        }

        *contactTime = (shortestTime * (1 - timeOfYIntersection)) + timeOfYIntersection;
        contactPoints->push_back(tempContactPoints[shortestTimeIndex]);
        return true;
    }

        return false;

}

bool CollisionSystem::narrowRayOnCuboid(const Collider* rayCollider, const Collider* cuboidCollider, std::vector<glm::vec3>* contactPoints, double* contactTime)
{
    RayShape* rayShape = (RayShape*)rayCollider->getShape();
    CuboidShape* cuboidShape = (CuboidShape*)cuboidCollider->getShape();

    //Cannot (and don't need to) perform directly up/down raycasts.
    if(rayShape->getDirection().x == 0 && rayShape->getDirection().z == 0)
    {
        return false;
    }

    Orientation startRay = rayCollider->getOrientation(0.0);
    Orientation endRay = rayCollider->getOrientation(1.0);

    Orientation startCuboid = cuboidCollider->getOrientation(0.0);
    Orientation endCuboid = cuboidCollider->getOrientation(1.0);

    glm::mat4 matrixEndRay = endRay.getOrientationMatrix();
    glm::mat4 matrixEndCuboid = endCuboid.getOrientationMatrix();

    glm::vec4 translatedRayOrigin = matrixEndRay * glm::vec4(rayShape->getOrigin(), 1.0f);
    glm::vec4 translateRayDest = matrixEndRay * glm::vec4(rayShape->getDirection()+rayShape->getOrigin(), 1.0f);

    glm::vec2 xzRayOrigin(translatedRayOrigin.x, translatedRayOrigin.z);
    glm::vec2 xzRayDest(translateRayDest.x, translateRayDest.z);



    //Build 2D square

    glm::vec3 corners[4];
    corners[0] = cuboidShape->getPos(); //Origin
    corners[1] = corners[0] + glm::vec3(cuboidShape->getXSize(), 0.0f, 0.0f);
    corners[2] = corners[1] + glm::vec3(0.0f, 0.0f, cuboidShape->getZSize());
    corners[3] = corners[0] + glm::vec3(0.0f, 0.0f, cuboidShape->getZSize());

    glm::vec2 corners_2D[4];

    for(unsigned int i = 0; i < 4; i++)
    {
        glm::vec4 translatedA = (matrixEndCuboid * glm::vec4(corners[i], 1.0f));
        corners_2D[i] = glm::vec2(translatedA.x, translatedA.z);
    }


    //Perform 2D raycast in XZ plane
    glm::vec2 collisionPoints[4];
    bool collided[4];

    for(unsigned int i = 0; i < 4; i++)
    {
        collided[i] = lineIntersectionInBounds(xzRayOrigin, xzRayDest, corners_2D[i], corners_2D[(i+1)%4], &collisionPoints[i]);
    }

    //Find Y values at points of intersection:
        //Ray's line equation is origin + dest * t
        // x = oX + (dX * t)
        // y = oY + (dY * t)
        // z = oZ + (dZ * t)

        //We have x and z, so we can get t:
        // t = (x - oX)/dX

        //With t we can get y

    float yValues[4];

    glm::vec4 cuboidPos_low = glm::vec4(cuboidShape->getPos(), 1.0f);
    glm::vec4 cuboidPos_high = glm::vec4(cuboidShape->getPos(), 1.0f);
    cuboidPos_high.y += cuboidShape->getYSize();

    float minY = (matrixEndCuboid * cuboidPos_low).y;
    float maxY = (matrixEndCuboid * cuboidPos_high).y;

    if(maxY < minY)
    {
        float temp = minY;
        minY = maxY;
        maxY = temp;
    }

    float deltaX = xzRayDest.x - xzRayOrigin.x;
    float deltaY = translateRayDest.y - translatedRayOrigin.y;
    float deltaZ = xzRayDest.y - xzRayOrigin.y;

    for(unsigned int i = 0; i < 4; i++)
    {
        if(collided[i])
        {
            float t;
            if(deltaX != 0) t = (collisionPoints[i].x - xzRayOrigin.x) / deltaX;
            else if(deltaZ != 0) t = (collisionPoints[i].y - xzRayOrigin.y) / deltaZ;
            yValues[i] = translatedRayOrigin.y + (deltaY * t);
            collided[i] = (yValues[i] <= maxY && yValues[i] >= minY);
        }
    }

    bool contact = false;

    unsigned int closest = 0;
    float closestDistance = rayShape->getDirection().length();;

    for(unsigned int i = 0; i < 4; i++)
    {
        if(collided[i])
        {
            float distance = glm::distance(translatedRayOrigin, glm::vec4(collisionPoints[i].x, yValues[i], collisionPoints[i].y, 1.0f));
            if(distance <= closestDistance)
            {
                closest = i;
                closestDistance = distance;
                contact = true;
            }

        }
    }

    *contactTime = 1.0;
    if(contact)
    {
        contactPoints->push_back(glm::vec3(collisionPoints[closest].x, yValues[closest], collisionPoints[closest].y));
        return true;
    }
    else
    {
        return false;
    }
    return contact;
}

bool CollisionSystem::lineIntersection(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2* result)
{
    *result = glm::vec2(0.0f, 0.0f);

    float deltaYA = (A2.y - A1.y);
    float deltaXA = (A2.x - A1.x);
    float deltaYB = (B2.y - B1.y);
    float deltaXB = (B2.x - B1.x);

    float x = 0.0f;
    float y = 0.0f;

    if((deltaXA == 0 && deltaXB == 0) || (deltaYA == 0 && deltaYB == 0))
    {
        return false;
    }
    else if(deltaYA == 0 && deltaYB != 0) //A is a straight horizontal line.
    {
        y = A1.y;
        float MB = deltaYB/deltaXB;
        x = ((y - B2.y)/MB) + B2.x;
    }
    else if(deltaXA == 0 && deltaXB != 0) //A is a straight vertical line.
    {
        x = A1.x;
        float MB = deltaYB/deltaXB;
        y = (MB * (x - B2.x)) + B2.y;
    }
    else if(deltaYB == 0 && deltaYA != 0)
    {
        y = B1.y;
        float MA = deltaYA/deltaXA;
        x = ((y - A2.y)/MA) + A2.x;
    }
    else if(deltaXB == 0 && deltaXA != 0)
    {
        x = B1.x;
        float MA = deltaYA/deltaXA;
        y = (MA * (x- A2.x)) + A2.y;
    }
    else
    {
        float MA = deltaYA/deltaXA;
        float MB = deltaYB/deltaXB;

        if(MA == MB) return false; //Lines are parrallel.

        float otherStuff = ((MA*A2.x) + A2.y) - ((MB*B2.x) + B2.y);

        x = otherStuff/(MA - MB);
        y = (MA * (x - A2.x)) + A2.y;
    }


    *result = glm::vec2(x, y);

    return true;
}

bool CollisionSystem::lineIntersectionInBounds(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2* result)
{
    *result = glm::vec2(0.0f, 0.0f);

    float deltaYA = (A2.y - A1.y);
    float deltaXA = (A2.x - A1.x);
    float deltaYB = (B2.y - B1.y);
    float deltaXB = (B2.x - B1.x);

    glm::vec2 pointOfIntersection(0.0f);

    bool linesIntersect = lineIntersection(A1, A2, B1, B2, &pointOfIntersection);

    if(!linesIntersect)
    {
        return false;
    }
    else
    {
        if(deltaYA >= 0 && (pointOfIntersection.y < A1.y || pointOfIntersection.y > A2.y)) return false;
        if(deltaYA <= 0 && (pointOfIntersection.y < A2.y || pointOfIntersection.y > A1.y)) return false;
        if(deltaXA >= 0 && (pointOfIntersection.x < A1.x || pointOfIntersection.x > A2.x)) return false;
        if(deltaXA <= 0 && (pointOfIntersection.x < A2.x || pointOfIntersection.x > A1.x)) return false;

        if(deltaYB >= 0 && (pointOfIntersection.y < B1.y || pointOfIntersection.y > B2.y)) return false;
        if(deltaYB <= 0 && (pointOfIntersection.y < B2.y || pointOfIntersection.y > B1.y)) return false;
        if(deltaXB >= 0 && (pointOfIntersection.x < B1.x || pointOfIntersection.x > B2.x)) return false;
        if(deltaXB <= 0 && (pointOfIntersection.x < B2.x || pointOfIntersection.x > B1.x)) return false;

        *result = pointOfIntersection;

        return true;
    }
}
