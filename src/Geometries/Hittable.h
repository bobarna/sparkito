#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct HitRecord { 
    Point3 p; 
    Vec3 normal; 
    REAL t; 
    bool front_face;

    // We save, whether we hit the inside of outside surface 
    void set_face_normal(const Ray& ray, 
                         const Vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit
        // length.
        
        // ray came from the outside if it points against the outward normal
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct Hittable {
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, 
                     REAL tmin, 
                     REAL tmax, 
                     HitRecord& rec) const = 0;
};

#endif
