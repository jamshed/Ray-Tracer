#include "plane.h"
#include <cstdio>
#include <cmath>

Plane::Plane()
{
}

Plane::Plane(Vector normal, double d)
{
    this->normal = normal, this->d = d;
}

Plane::Plane(Point a, Point b, Point c)
{
    Vector va(a), vb(b), vc(c);
    normal = (va-vb).cross(vb-vc);
    d = -(normal.p.x*a.x+normal.p.y*a.y+normal.p.z*a.z);
}

RayIntersection Plane::ray_intersection(Ray &ray)
{
    Vector m;

    if(!(ray.direction.dot(this->normal)<0))
        m = -this->normal;
    else
        m = this->normal;

    double dot = normal.dot(ray.direction);
	if(abs(dot)<1e-5)
        return RayIntersection (-1, Vector());

    double t = -(d+normal.dot(ray.origin))/dot;

    if(t<0)
        return RayIntersection(t, Vector());

    return RayIntersection(t, m);
}

