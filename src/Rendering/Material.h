#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Sparkito.h"

struct HitRecord;

class Material{
public:
    virtual ~Material() = default;
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        Color& attenuation,
        Ray& scattered
    ) const = 0;
};

#endif #MATERIAL_H
