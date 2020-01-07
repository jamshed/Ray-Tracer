#include <cmath>
#include "ray_tracer.h"

RayTracer::RayTracer()
{
}

RayTracer::RayTracer(vector<Shape *> &shapes, vector<Point> &lights)
{
    EPS = 1e-9, INF = 1000000000000000000.0, LIGHT_INTENSITY = 1.0/3;
    this->shapes = shapes, this->lights = lights;
}

Ray RayTracer::get_initial_ray(int dimension, int x, int y, Camera &camera)
{
    double normalizedX = (double(x)/dimension)-0.5;
	double normalizedY = (double(-y)/dimension)+0.5;
	Vector rayDirection = (camera.rightwards*normalizedX)+(camera.upwards*normalizedY)+(-camera.backwards)*0.71;

	camera.reload();

	return Ray(camera.position, rayDirection);
}

RayHit RayTracer::closest_hit(Ray &ray)
{
    RayHit closestHit(RayIntersection(INF, Vector()), 0);

    for(int i = 0; i<shapes.size(); ++i)
    {
        RayIntersection rayIntersection = shapes[i]->ray_intersection(ray);
        if(rayIntersection.t<0)
            continue;

        if(closestHit.rayIntersection.t>rayIntersection.t)
            closestHit = RayHit(rayIntersection, shapes[i]);
    }

    return closestHit;
}

Color RayTracer::trace_ray(Ray ray)
{
    RayHit closestHit = closest_hit(ray);

    if(closestHit.rayIntersection.t<INF)
    {
        Point p = (ray.origin+ray.direction*closestHit.rayIntersection.t).p;
        return closestHit.shape->get_color(p);
    }

    return Color(0, 0, 0);
}

void RayTracer::render_initial_image(int dimension, vector<vector<Color> > &pixelMatrix, Camera &camera)
{
    for(int i = 0; i<dimension; ++i)
        for(int j = 0; j<dimension; ++j)
        {
            //Ray ray = get_initial_ray(dimension, i, j, camera);
            //printf("Ray origin = "), ray.origin.print();
            //printf("Ray dir = "), ray.direction.print();

            pixelMatrix[i][j] = trace_ray(get_initial_ray(dimension, i, j, camera));
        }
}

bool RayTracer::can_reach_light_source(Ray &ray)
{
    double &ox = ray.origin.p.x, &oy = ray.origin.p.y, &oz = ray.origin.p.z,
           &dx = ray.direction.p.x, &dy = ray.direction.p.y, &dz = ray.direction.p.z;

    if(fabs(dx)<EPS || fabs(dy)<EPS || fabs(dz)<EPS)
    {
        for(int i = 0; i<lights.size(); ++i)
        {
            double &lx = lights[i].x, &ly = lights[i].y, &lz = lights[i].z;
            if(fabs(ox-lx)<EPS && fabs(oy-ly)<EPS && fabs(oz-lz)<EPS)
                return true;
        }

        return false;
    }

    int i, j;

    for(i = 0; i<lights.size(); ++i)
    {
        double &lx = lights[i].x, &ly = lights[i].y, &lz = lights[i].z;
        double tx = (lx-ox)/dx, ty = (ly-oy)/dy, tz = (lz-oz)/dz;

        if(fabs(tx-ty)<EPS && fabs(ty-tz)<EPS && fabs(tz-tx)<EPS)
        {
            for(j = 0; j<shapes.size(); ++j)
            {
                RayIntersection intersection = shapes[j]->ray_intersection(ray);
                if(intersection.t<0)
                    continue;

                if(intersection.t<tx)
                    break;
            }

            if(j==shapes.size())
                return true;
        }
    }

    return false;
}

Color RayTracer::color_from_light_source(Ray &incidentRay, Ray &shadowRay, Vector &normal,
                                         Shape *shape)
{
    double &difCoeff = shape->difCoeff, &specCoeff = shape->specCoeff, &specExp = shape->specExp;

    Vector s = shadowRay.direction, v = -incidentRay.direction, &m = normal;
    Vector r = (-s).reflection(m);

    double lambert = max(0.0, s.dot(m)/(s.length()*m.length())),
           phong = max(0.0, pow(r.dot(v)/(r.length()*v.length()), specExp));

    return (shape->get_color(shadowRay.origin.p)*(LIGHT_INTENSITY*difCoeff*lambert))+
           Color(1, 1, 1)*(LIGHT_INTENSITY*specCoeff*phong);
}

Color RayTracer::trace_ray(Ray ray, int recDepth)
{
    if(!recDepth)
        return Color(0, 0, 0);

    RayHit closestHit = closest_hit(ray);
    double &t = closestHit.rayIntersection.t;
    Vector &normal = closestHit.rayIntersection.normal;
    Shape *shape = closestHit.shape;
    Color color(0, 0, 0);

    if(t<INF)
    {
        Vector hit = ray.origin+ray.direction*t;
        color = shape->get_color(hit.p)*shape->ambCoeff;

        for(int i = 0; i<lights.size(); ++i)
        {
            Ray shadowRay = Ray(hit, Vector(lights[i])-hit).shift_origin(0.001);

            if(can_reach_light_source(shadowRay))
                color += color_from_light_source(ray, shadowRay, normal, shape);
        }

        color += (trace_ray(ray.reflected_ray(hit, normal).shift_origin(0.001), recDepth-1)*
                  shape->refCoeff);
    }

    return color;
}

void RayTracer::trace_rays(int dimension, int recDepth, vector<vector<Color> > &pixelMatrix,
                           Camera &camera)
{
    for(int i = 0; i<dimension; ++i)
        for(int j = 0; j<dimension; ++j)
            pixelMatrix[i][j] = trace_ray(get_initial_ray(dimension, i, j, camera), recDepth);
}
