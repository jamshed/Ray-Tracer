#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:

    Vector origin, direction;

    Ray();
    Ray(Vector origin, Vector direction);
    Ray reflected_ray(Vector &incidence, Vector &normal);
    Ray shift_origin(double t);
};


#endif // RAY_H
