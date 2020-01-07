#include "point.h"

Point::Point()
{
}

Point::Point(double x, double y, double z)
{
    this->x = x, this->y = y, this->z = z;
}

Point Point::operator -()
{
    return Point(-x, -y, -z);
}
