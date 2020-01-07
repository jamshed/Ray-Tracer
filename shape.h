#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include "point.h"
#include "ray.h"
#include "color.h"
#include "plane.h"
#include "ray_intersection.h"

class Shape
{
public:

    double ambCoeff, difCoeff, refCoeff, specCoeff, specExp;

    Shape();
    Shape(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp);
    virtual Color get_color(Point &point) = 0;
    virtual RayIntersection ray_intersection(Ray &ray) = 0;
};

class Checkerboard : public Shape
{
public:

    int c, dimension;
    Plane plane;
    Color color[2];

    Checkerboard();
    Checkerboard(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
                 Color color1, Color color2);
    virtual Color get_color(Point &point);
    virtual RayIntersection ray_intersection(Ray &ray);
};

class Sphere : public Shape
{
public:

    Point center;
    double radius;
    Color color;

    Sphere();
    Sphere(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
           Point center, double radius, Color color);
    virtual Color get_color(Point &point);
    virtual RayIntersection ray_intersection(Ray &ray);
};

class Cylinder : public Shape
{
public:

    double xCenter, zCenter, yMin, yMax, radius;
    Color color;
    Plane planeTop, planeBot;

    Cylinder();
    Cylinder(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
             double xCenter, double zCenter, double yMin, double yMax, double radius, Color color);
    virtual Color get_color(Point &point);
    virtual RayIntersection ray_intersection(Ray &ray);
};

class Triangle : public Shape
{
public:

    Point a, b, c;
    Color color;
    double refractiveIndex;
    Plane plane;

    Triangle();
    Triangle(double ambCoeff, double difCoeff, double refCoeff, double specCoeff, double specExp,
             Point a, Point b, Point c, Color color, double refractiveIndex);
    virtual Color get_color(Point &point);
    virtual RayIntersection ray_intersection(Ray &ray);
};

#endif // SHAPE_H
