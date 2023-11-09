#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "../Sparkito.h"

class Sphere : public Hittable {
    Point3 center;
    REAL radius;
public:
    Sphere(
        Point3 _center, 
        REAL _radius
    ) : center(_center), 
        radius(_radius) {}

    bool hit(const Ray& ray, 
             REAL tmin, 
             REAL tmax, 
             HitRecord& rec) const override {
        Vec3 co = ray.origin() - center;
        // Solving the quadratic equation for determining intersection
        // between the ray and the sphere
        REAL a = ray.direction().length_squared();
        REAL half_b = dot(co, ray.direction());
        REAL c = co.length_squared() - radius*radius;

        REAL discriminant = half_b*half_b - a*c;

        if (discriminant < 0) return false;

        // find the nearest solution (root) that lies in the acceptable range
        REAL sqrtd = sqrt(discriminant);
        REAL t_hit = ( -half_b - sqrtd ) / a;
        if (t_hit <= tmin || t_hit >= tmax) {
            t_hit = ( -half_b - sqrtd ) / a;
            if (t_hit <= tmin || t_hit >= tmax) 
                return false;
        }

        // "save out" HitRecord data
        rec.t = t_hit;
        rec.p = ray.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(ray, outward_normal);

        return true;
    }
};

#endif
