#include "ray.h"

Ray::Ray()
{
}

Ray::Ray(Vector origin, Vector direction)
{
    this->origin = origin, this->direction = direction;
    this->direction.normalize();
}

Ray Ray::reflected_ray(Vector &incidence, Vector &normal)
{
    Vector reflectedDir = direction.reflection(normal);
    return Ray(incidence, reflectedDir);
}

Ray Ray::shift_origin(double t)
{
    Vector shiftedOrigin = origin+direction*t;
    return Ray(shiftedOrigin, direction);
}

