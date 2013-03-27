#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Orientation
{
    public:
        Orientation();
        virtual ~Orientation();

        //Static Functions
        //Mix two orientations by a certain ratio: O1 * (1 - ratio) + ratio * O2
        //So 0 gives orientation1, and 1 gives orientation2
        static Orientation mix(const Orientation& orientation1, const Orientation& orientation2, const double ratio);

        void pitch(GLfloat angle);
        void yaw(GLfloat angle);
        void roll(GLfloat angle);

        void fly(GLfloat amount);
        void strafe(GLfloat amount);
        void walk(GLfloat amount);

        //Translate the orinentation, NOT using it's direction vectors.
        void translate(GLfloat x, GLfloat y, GLfloat z);

        //Transform the orientation by a matrix and attempt to keep vectors and position valid.
        void transform(const glm::mat4& transform);

        GLfloat getPitch() const;
        GLfloat getYaw() const;
        GLfloat getRoll() const;

        glm::vec3 getPos() const;
        glm::vec3 getUp() const;
        glm::vec3 getRight() const;
        glm::vec3 getForward() const;

        void setPos(glm::vec3 pos);
        void setUp(glm::vec3 up);
        void setRight(glm::vec3 right);
        void setForward(glm::vec3 forward);

        glm::mat4 getOrientationMatrix() const;

        void reset();
        void resetRotation();

    protected:

        GLfloat mPitch;
        GLfloat mYaw;
        GLfloat mRoll;

        glm::vec3 mPos;
        glm::vec3 mForward;
        glm::vec3 mUp;
        glm::vec3 mRight;

        glm::mat4 mOrientation;
        bool mMatrixChanged;

        GLboolean mCanRoll;

    private:
};

#endif // ORIENTATION_H
