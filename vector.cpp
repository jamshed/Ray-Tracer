#include "vector.h"
#include <cmath>

Vector::Vector()
{
}

Vector::Vector(Point p)
{
    this->p = p;
}

Vector::Vector(double x, double y, double z)
{
    p = Point(x, y, z);
}

double Vector::length()
{
    return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}

void Vector::normalize()
{
    double len = length();
    p.x /= len, p.y /= len, p.z /= len;
}

Vector Vector::operator -()
{
    return Vector(-p.x, -p.y, -p.z);
}

Vector Vector::operator +(Vector vec)
{
    return Vector(p.x+vec.p.x, p.y+vec.p.y, p.z+vec.p.z);
}

Vector Vector::operator -(Vector vec)
{
    return Vector(p.x-vec.p.x, p.y-vec.p.y, p.z-vec.p.z);
}

Vector Vector::operator *(double scalar)
{
    return Vector(p.x*scalar, p.y*scalar, p.z*scalar);
}

double Vector::dot(Vector vec)
{
    return p.x*vec.p.x+p.y*vec.p.y+p.z*vec.p.z;
}

Vector Vector::cross(Vector vec)
{
    double &ax = p.x, &ay = p.y, &az = p.z, &bx = vec.p.x, &by = vec.p.y, &bz = vec.p.z;

    return Vector(ay*bz-az*by, az*bx-ax*bz, ax*by-ay*bx);
}

Vector Vector::translate(Point origin)
{
    return Vector(p.x-origin.x, p.y-origin.y, p.z-origin.z);
}

Vector Vector::reflection(Vector &normal)
{
    return *this-normal*(2*dot(normal));
}

void Vector::print()
{
    printf("%f %f %f\n", p.x, p.y, p.z);
}
