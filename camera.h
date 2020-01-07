#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include "vector.h"

class Camera
{
public:

	float m[16];
	Vector backwards, upwards, rightwards, position, lookAt, up;

	Camera();
	void reset();
	void reload();
	Vector get_forward_vector();
	Vector get_right_vector();
	Vector get_up_vector();
	Point get_eye_position();
	void walk(double units);
	void yaw(double rotationAngle);
	void pitch(double rotationAngle);
	void roll(double rotationAngle);
	void strafe(double units);
	void fly(double units);
};

#endif // CAMERA_H
