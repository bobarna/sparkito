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

#endif //MATERIAL_H
