#ifndef VECTOR_H
#define VECTOR_H

#include <cstdio>
#include "point.h"

class Vector
{
public:

    Point p;

    Vector();
    Vector(Point p);
    Vector(double x, double y, double z);
    double length();
    void normalize();
    Vector operator -();
    Vector operator +(Vector vec);
    Vector operator -(Vector vec);
    Vector operator *(double scalar);
    double dot(Vector vec);
    Vector cross(Vector vec);
    Vector translate(Point origin);
    Vector reflection(Vector &normal);
    void print();
};

#endif // VECTOR_H
