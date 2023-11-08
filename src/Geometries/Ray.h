#ifndef RAY_H
#define RAY_H

#include "../Math/MATH_DEFS.h"
#include "../Math/Vec3.h"

class Ray {
    public:
        Ray() {}

        Ray(const Point3& origin, const Vec3& direction) 
            : p(origin), dir(direction) {}
        
        Point3 origin() const { return p; }
        Vec3 direction() const { return dir; }

        Point3 at(REAL t) const {
            return p + t*dir;
        }

    private:
        Point3 p;
        Vec3 dir;
};

#endif
