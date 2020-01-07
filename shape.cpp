#include <cmath>
#include <algorithm>
#include "shape.h"

Shape::Shape()
{
}

Shape::Shape(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp)
{
    this->ambCoeff = ambCoeff, this->difCoeff = difCoeff, this->refCoeff = refCoeff,
    this->specCoeff = specCoeff, this->specExp = specExp;
}

Checkerboard::Checkerboard()
{
}

Checkerboard::Checkerboard(double ambCoeff, double difCoeff, double refCoeff,
                           double specCoeff, double specExp, Color color1, Color color2) :
                               Shape(ambCoeff, difCoeff, refCoeff, specCoeff, specExp)
{
    c = 250, dimension = 20;
    color[0] = color1, color[1] = color2;
    plane = Plane(Point(0, 0, 0), Point(1, 0, 1), Point(2, 0, 4));
}

Color Checkerboard::get_color(Point &point)
{
    double x = point.x/dimension, z = point.z/dimension;
    if(x<0)
        x--;
    if(z<0)
        z--;

    int i = (int)x, k = (int)z;

    return color[(i&1)==(k&1)];
}

RayIntersection Checkerboard::ray_intersection(Ray &ray)
{
    return plane.ray_intersection(ray);
}

Sphere::Sphere()
{
}

Sphere::Sphere(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
               Point center, double radius, Color color) :
                   Shape(ambCoeff, difCoeff, refCoeff, specCoeff, specExp)
{
    this->center = center, this->radius = radius, this->color = color;
}

Color Sphere::get_color(Point &point)
{
    return color;
}

RayIntersection Sphere::ray_intersection(Ray &ray)
{
    Ray translatedRay = Ray(ray.origin.translate(center), ray.direction);

    double a = 1,
           b = 2*translatedRay.direction.dot(translatedRay.origin),
           c = translatedRay.origin.dot(translatedRay.origin)-radius*radius;

    if(b*b-4*a*c<0)
    {
        //printf("Invalid sphere determinant\n");
        return RayIntersection(-1, Vector());
    }

    //printf("valid determinant\n");

    double d = sqrt(b*b-4*a*c);
    double t1 = (-b+d)/(2*a), t2 = (-b-d)/(2*a);
    double t = (t1<0 ? t2 : (t2<0 ? t1 : std::min(t1, t2)));

    if(t<0)
    {
        //printf("Invalid intersection\n");
        return RayIntersection(t, Vector());
    }

    return RayIntersection(t, translatedRay.origin+translatedRay.direction*t);
}

Cylinder::Cylinder()
{
}

Cylinder::Cylinder(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
                   double xCenter, double zCenter, double yMin, double yMax, double radius,
                   Color color) : Shape(ambCoeff, difCoeff, refCoeff, specCoeff, specExp)
{
    this->xCenter = xCenter, this->zCenter = zCenter, this->yMin = yMin, this->yMax = yMax,
    this->radius = radius, this->color = color;

    planeTop = Plane(Point(0, yMax, 0), Point(1, yMax, 1), Point(2, yMax, 4)),
    planeBot = Plane(Point(0, yMin, 0), Point(1, yMin, 1), Point(2, yMin, 4));
}

Color Cylinder::get_color(Point &point)
{
    return color;
}

RayIntersection Cylinder::ray_intersection(Ray &ray)
{
    double &ox = ray.origin.p.x, &oz = ray.origin.p.z,
           &dx = ray.direction.p.x, &dz = ray.direction.p.z,
           &cx = xCenter, &cz = zCenter,
           &r = radius;

    double a = dx*dx+dz*dz,
           b = 2*(dx*(ox-cx)+dz*(oz-cz)),
           c = (ox-cx)*(ox-cx)+(oz-cz)*(oz-cz)-r*r, d, t1, t2, t;

    if(b*b-4*a*c<0)
    {
        //printf("invalid determinant\n");
        t = -1;
    }
    else
    {
        d = sqrt(b*b-4*a*c);
        t1 = (-b+d)/(2*a), t2 = (-b-d)/(2*a);
        t = (t1<0 ? t2 : (t2<0 ? t1 : std::min(t1, t2)));

        //printf("t =%f\n", t);

        if(!(t<0))
        {
            //printf("valid intersection\n");

            Point intPoint = (ray.origin+ray.direction*t).p;

            if(!(intPoint.y<yMin) && !(intPoint.y>yMax))
                return RayIntersection(t, Vector(intPoint)-Vector(xCenter, intPoint.y, zCenter));
        }
    }

    RayIntersection intersection = planeTop.ray_intersection(ray);
    t = intersection.t;
    if(!(t<0))
    {
        Point intPoint = (ray.origin+ray.direction*t).p;
        d = (intPoint.x-xCenter)*(intPoint.x-xCenter)+(intPoint.z-zCenter)*(intPoint.z-zCenter);
        if(!(d>r*r))
            return RayIntersection(t, planeTop.normal);
    }

    intersection = planeBot.ray_intersection(ray);
    t = intersection.t;
    if(!(t<0))
    {
        Point intPoint = (ray.origin+ray.direction*t).p;
        d = (intPoint.x-xCenter)*(intPoint.x-xCenter)+(intPoint.z-zCenter)*(intPoint.z-zCenter);
        if(!(d>r*r))
            return RayIntersection(t, planeBot.normal);
    }

    return RayIntersection(-1, Vector());
}

Triangle::Triangle()
{
}

Triangle::Triangle(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
                   Point a, Point b, Point c, Color color, double refractiveIndex) :
                       Shape(ambCoeff, difCoeff, refCoeff, specCoeff, specExp)
{
    this->a = a, this->b = b, this->c = c, this->color = color,
    this->refractiveIndex = refractiveIndex;

    plane = Plane(a, b, c);
}

Color Triangle::get_color(Point &point)
{
    return color;
}

RayIntersection Triangle::ray_intersection(Ray &ray)
{
    RayIntersection intersection = plane.ray_intersection(ray);
    double &t = intersection.t;

    if(t<0)
        return intersection;

    //puts("HERE");

    Point intPoint = (ray.origin+ray.direction*t).p;
    Vector va(a), vb(b), vc(c), vp(intPoint);
    double dot1 = (vb-va).cross(vp-va).dot(plane.normal),
           dot2 = (vc-vb).cross(vp-vb).dot(plane.normal),
           dot3 = (va-vc).cross(vp-vc).dot(plane.normal);

    bool nonneg1 = !(dot1<0), nonpos1 = !(dot1>0),
         nonneg2 = !(dot2<0), nonpos2 = !(dot2>0),
         nonneg3 = !(dot3<0), nonpos3 = !(dot3>0);

    if((nonneg1 && nonneg2 && nonneg3) || (nonpos1 && nonpos2 && nonpos3))
        return intersection;

    return RayIntersection(-1, Vector());
}
