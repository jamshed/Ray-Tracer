#ifndef RAY_INTERSECTION_H
#define RAY_INTERSECTION_H

#include "vector.h"

class RayIntersection
{
public:

    double t;
    Vector normal;

    RayIntersection();
    RayIntersection(double t, Vector normal);
};

#endif // RAY_INTERSECTION_H
