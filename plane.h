#ifndef PLANE_H
#define PLANE_H

#include "vector.h"
#include "ray.h"
#include "ray_intersection.h"

class Plane
{
public:

    Vector normal;
    double d;

    Plane();
    Plane(Vector normal, double d);
    Plane(Point a, Point b, Point c);
    RayIntersection ray_intersection(Ray &ray);
};

#endif // PLANE_H
