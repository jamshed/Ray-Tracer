#include "ray_hit.h"

RayHit::RayHit()
{
}

RayHit::RayHit(RayIntersection rayIntersection, Shape *shape)
{
    this->rayIntersection = rayIntersection, this->shape = shape;
}
