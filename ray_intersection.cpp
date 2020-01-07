#include "ray_intersection.h"

RayIntersection::RayIntersection()
{
}

RayIntersection::RayIntersection(double t, Vector normal)
{
    this->t = t, this->normal = normal;
}
