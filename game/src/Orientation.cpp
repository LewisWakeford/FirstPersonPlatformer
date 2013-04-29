#include "Orientation.h"

Orientation Orientation::mix(const Orientation& orientation1, const Orientation& orientation2, const double ratio)
{
    Orientation result;
    result.setForward(glm::mix(orientation1.getForward(), orientation2.getForward(), ratio));
    result.setUp(glm::mix(orientation1.getUp(), orientation2.getUp(), ratio));
    result.setRight(glm::mix(orientation1.getRight(), orientation2.getRight(), ratio));
    result.setPos(glm::mix(orientation1.getPos(), orientation2.getPos(), ratio));
    return result;
}

Orientation::Orientation()
{
    reset();
}

Orientation::~Orientation()
{
    //dtor
}

glm::vec3 Orientation::getForward() const
{
    return mForward;
}

glm::vec3 Orientation::getRight() const
{
    return mRight;
}

glm::vec3 Orientation::getUp() const
{
    return mUp;
}

GLfloat Orientation::getPitch() const
{
    return mPitch;
}

GLfloat Orientation::getYaw() const
{
    return mYaw;
}

GLfloat Orientation::getRoll() const
{
    return mRoll;
}

glm::vec3 Orientation::getPos() const
{
    return mPos;
}

void Orientation::setPos(glm::vec3 pos)
{
    mPos = pos;
}

void Orientation::setUp(glm::vec3 up)
{
    mUp = up;
}

void Orientation::setRight(glm::vec3 right)
{
    mRight = right;
}

void Orientation::setForward(glm::vec3 forward)
{
    mForward = forward;
}


void Orientation::yaw(GLfloat angle)
{
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, mUp);

    mRight = glm::vec3(rotMat * glm::vec4(mRight, 1.0f));
    mForward =  glm::vec3(rotMat * glm::vec4(mForward, 1.0f));

    mRight = glm::normalize(mRight);
    mForward = glm::normalize(mForward);
}

void Orientation::pitch(GLfloat angle)
{
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, mRight);

    mUp =  glm::vec3(rotMat * glm::vec4(mUp, 1.0f));
    mForward =  glm::vec3(rotMat * glm::vec4(mForward, 1.0f));

    mUp = glm::normalize(mUp);
    mForward = glm::normalize(mForward);

}

void Orientation::roll(GLfloat angle)
{
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, mForward);

    mUp =  glm::vec3(rotMat * glm::vec4(mUp, 1.0f));
    mRight =  glm::vec3(rotMat * glm::vec4(mRight, 1.0f));

    mUp = glm::normalize(mUp);
    mRight = glm::normalize(mRight);
}

void Orientation::fly(GLfloat amount)
{
    mPos.x += mUp.x * amount;
    mPos.y += mUp.y * amount;
    mPos.z += mUp.z * amount;
}

void Orientation::strafe(GLfloat amount)
{
    mPos.x += mRight.x * amount;
    mPos.y += mRight.y * amount;
    mPos.z += mRight.z * amount;
}

void Orientation::walk(GLfloat amount)
{
    mPos.x -= mForward.x * amount;
    mPos.y -= mForward.y * amount;
    mPos.z -= mForward.z * amount;
}

glm::mat4 Orientation::getOrientationMatrix() const
{
    /*
    glm::mat4 rotMat(mRight.x, mUp.x, mForward.x, 0,
                  mRight.y,  mUp.y,  mForward.y, 0,
                  mRight.z,  mUp.z,  mForward.z, 0,
                  0, 0, 0, 1);
    */

    glm::mat4 rotMat(mRight.x, mRight.y, mRight.z, 0,
                  mUp.x,  mUp.y,  mUp.z, 0,
                  mForward.x,  mForward.y,  mForward.z, 0,
                  0, 0, 0, 1);

    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), mPos);

    glm::mat4 result = transMat * rotMat;

    return result;
}

void Orientation::reset()
{
    mPitch = 0;
    mYaw = 0;
    mRoll = 0;

    mPos = glm::vec3(0.0f,0.0f,0.0f);
    mForward = glm::vec3(0.0f,0.0f,1.0f);
    mUp = glm::vec3(0.0f,1.0f,0.0f);
    mRight = glm::vec3(1.0f,0.0f,0.0f);
}

void Orientation::resetRotation()
{
    mForward = glm::vec3(0.0f,0.0f,1.0f);
    mUp = glm::vec3(0.0f,1.0f,0.0f);
    mRight = glm::vec3(1.0f,0.0f,0.0f);
}

void Orientation::translate(GLfloat x, GLfloat y, GLfloat z)
{
    mPos.x += x;
    mPos.y += y;
    mPos.z += z;
}

void Orientation::translate(const glm::vec3& trans)
{
    mPos += trans;
}

void Orientation::transform(const glm::mat4& transform)
{
    mPos = glm::vec3(transform * glm::vec4(mPos, 1.0f));
    glm::mat3 rotationMatrix = glm::mat3(transform);
    mUp = rotationMatrix * mUp;
    mRight = rotationMatrix * mRight;
    mForward = rotationMatrix * mForward;
    mUp = glm::normalize(mUp);
    mRight = glm::normalize(mRight);
    mForward = glm::normalize(mForward);
}
