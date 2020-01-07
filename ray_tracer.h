#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <vector>
#include "point.h"
#include "color.h"
#include "shape.h"
#include "ray.h"
#include "ray_intersection.h"
#include "ray_hit.h"
#include "camera.h"
using namespace std;

class RayTracer
{
public:

    double EPS, INF, LIGHT_INTENSITY;
    vector<Point> lights;
    vector<Shape *> shapes;

    RayTracer();
    RayTracer(vector<Shape *> &shapes, vector<Point> &lights);
    Ray get_initial_ray(int dimension, int x, int y, Camera &camera);
    RayHit closest_hit(Ray &ray);
    void render_initial_image(int dimension, vector<vector<Color> > &pixelMatrix, Camera &camera);
    bool can_reach_light_source(Ray &ray);
    Color color_from_light_source(Ray &incidentRay, Ray &deflectedRay, Vector &normal, Shape *shape);
    Color trace_ray(Ray ray);
    Color trace_ray(Ray ray, int recDepth);
    void trace_rays(int dimension, int recDepth, vector<vector<Color> > &pixelMatrix, Camera &camera);
};

#endif // RAY_TRACER_H
