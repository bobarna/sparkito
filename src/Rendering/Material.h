#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Sparkito.h"

struct HitRecord;

class Material{

public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& hit,
        Color& attenuation,
        Ray& scattered
    ) const = 0;

};

class LambertianMaterial : public Material {

    Color albedo;
public:
    LambertianMaterial(const Color& _albedo) : albedo(_albedo) {}

    bool scatter(
        const Ray&, // ray_in is not used in our Lambertian model
        const HitRecord& hit,
        Color& attenuation,
        Ray& scattered
    ) const override {
        Vec3 scatter_direction = hit.normal + get_random_unit_vector();

        // Catch erroneous case, where random vector pointed exactly opposite of
        // the surface normal vector
        if (scatter_direction.near_zero())
            scatter_direction = hit.normal;

        scattered = Ray(hit.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

};

class MetalMaterial : public Material {
    // Perfectly reflective material

Color albedo;
REAL fuzziness;
public:
    MetalMaterial(const Color& _albedo, REAL _fuzziness = 0.0) 
        : albedo(_albedo),
          fuzziness(_fuzziness) {}

    bool scatter(
        const Ray& ray_in,
        const HitRecord& hit,
        Color& attenuation,
        Ray& scattered
    ) const override {
        Vec3 reflected_dir = reflect(
                unit_vector( ray_in.direction() ), 
                hit.normal
        );

        // add fuzziness to reflection
        reflected_dir += get_random_unit_vector() * fuzziness;

        scattered = Ray(hit.p, reflected_dir);
        attenuation = albedo;

        bool fuzzed_below_surface = (dot(reflected_dir, hit.normal) <= 0);

        return !fuzzed_below_surface;
    }
};

class DielectricMaterial : public Material {
    // Dielectric material that always refracts
    REAL ior; // Index of Refraction; eta_incoming / eta_refracted
public:
    DielectricMaterial(REAL _ior) 
        : ior(_ior) {}

    bool scatter(
        const Ray& ray_in,
        const HitRecord& hit,
        Color& attenuation,
        Ray& scattered
    ) const override {
        // don't change color
        attenuation = Color(1.0, 1.0, 1.0);
        REAL refraction_ratio = hit.front_face ? (1.0/ior) : ior;
        
        Vec3 in_dir = unit_vector(ray_in.direction());
        REAL cos_theta = fmin(dot(-in_dir, hit.normal), 1.0);
        REAL sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        REAL cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 out_dir;

        if(cannot_refract)
            out_dir = reflect(in_dir, hit.normal);
        else
            out_dir = refract(in_dir, hit.normal, refraction_ratio);

        scattered = Ray(hit.p, out_dir);
        return true;
    }

};

#endif //MATERIAL_H
