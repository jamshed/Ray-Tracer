#include "camera.h"

Camera::Camera()
{
    reset();
}

void Camera::reset()
{
    position = Vector(80, 35, 0);
    lookAt = Vector(0, 35, 0);
    up = Vector(0, 1, 0);
}

void Camera::reload()
{
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    backwards = Vector(m[2], m[6], m[10]), upwards = Vector(m[1], m[5], m[9]),
    rightwards = Vector(m[0], m[4], m[8]);

    backwards.normalize(), upwards.normalize(), rightwards.normalize();
}

Vector Camera::get_forward_vector()
{
    return -backwards;
}

Vector Camera::get_right_vector()
{
    return rightwards;
}

Vector Camera::get_up_vector()
{
    return upwards;
}

Point Camera::get_eye_position()
{
    return Point(position.p.x, position.p.y, position.p.z);
}

void Camera::walk(double units)
{
    reload();
    Vector d = backwards*units;
    position = position+d;
    lookAt = lookAt+d;
}

void Camera::yaw(double rotationAngle)
{
    glPushMatrix();
    {
        glRotatef(rotationAngle, upwards.p.x, upwards.p.y, upwards.p.z);
        reload();
    }
    glPopMatrix();

    lookAt = position-backwards;
}

void Camera::pitch(double rotationAngle)
{
    glPushMatrix();
    {
        glRotatef(rotationAngle, rightwards.p.x, rightwards.p.y, rightwards.p.z);
        reload();
    }
    glPopMatrix();

    lookAt = position-backwards;
    up = upwards;
}

void Camera::roll(double rotationAngle)
{
    glPushMatrix();
    {
        glRotatef(rotationAngle, backwards.p.x, backwards.p.y, backwards.p.z);
        reload();
    }
    glPopMatrix();

    up = upwards;
}

void Camera::strafe(double units)
{
    reload();

    Vector d = rightwards*units;
    position = position+d;
    lookAt = lookAt+d;
}

void Camera::fly(double units)
{
    reload();

    Vector d = upwards*units;
    position = position+d;
    lookAt = lookAt+d;
}
