#ifndef RAY_HIT_H
#define RAY_HIT_H

#include "shape.h"
#include "ray_intersection.h"

class RayHit
{
public:

    RayIntersection rayIntersection;
    Shape *shape;

    RayHit();
    RayHit(RayIntersection rayIntersection, Shape *shape);
};

#endif // RAY_HIT_H
