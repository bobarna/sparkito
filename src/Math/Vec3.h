#ifndef VEC3_H
#define VEC3_H

#include "../Sparkito.h"

class Vec3 {
    public:
        REAL e[3];

        Vec3() : e{0,0,0} {}
        Vec3(REAL e0, REAL e1, REAL e2) : e{e0, e1, e2} {}

        REAL x() const { return e[0]; }
        REAL y() const { return e[1]; }
        REAL z() const { return e[2]; }

        REAL r() const { return e[0]; }
        REAL g() const { return e[1]; }
        REAL b() const { return e[2]; }

        REAL u() const { return e[0]; }
        REAL v() const { return e[1]; }
        REAL w() const { return e[2]; }

        Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
        
        REAL operator[](int i) const { return e[i]; }

        REAL& operator[](int i) { return e[i]; }

        Vec3& operator+=(const Vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vec3& operator*=(REAL t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec3& operator/=(REAL t) { return *this *= 1/t; }

        REAL length() const { return sqrt(length_squared()); }

        REAL length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            REAL eps = 1e-8;
            return (
                ( fabs(e[0]) < eps ) 
                && ( fabs(e[1]) < eps ) 
                && ( fabs(e[2]) < eps )
            );
        }

        static Vec3 get_random() {
            return Vec3(random_real(), random_real(), random_real());
        }

        static Vec3 get_random(REAL min, REAL max) {
            return Vec3(
                random_real(min, max), 
                random_real(min, max), 
                random_real(min, max)
            );
        }
};

// Point3 is just an alias for Vec3; useful for clarity in the code.
using Point3 = Vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' '  << v[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0]*v[0], u[1]*v[1], u[2]*v[2]);
}

inline Vec3 operator*(REAL t, const Vec3 &v) {
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator*(const Vec3 &v, REAL t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, REAL t) {
    return (1/t) * v;
}

inline REAL dot(const Vec3 &u, const Vec3 &v) {
    return u[0] * v[0]
         + u[1] * v[1]
         + u[2] * v[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[1] * v[2] - u[2]*v[1],
                u[2] * v[0] - u[0]*v[2],
                u[0] * v[1] - u[1]*v[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline Vec3 get_random_in_unit_sphere() {
    while(true)  {
        auto p = Vec3::get_random(-1, 1);
        if(p.length_squared() < 1)
            return p;
    }
}

inline Vec3 get_random_unit_vector() {
    return unit_vector(get_random_in_unit_sphere());
}

inline Vec3 get_random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = get_random_in_unit_sphere();
    if(dot(on_unit_sphere, normal) > 0.0) 
        // in the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}


#endif



