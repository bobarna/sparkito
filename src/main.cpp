#include <iostream>

#include "Sparkito.h"
#include "Math/Vec3.h"
#include "Utils/Color.h"
#include "Geometries/Ray.h"
#include "Geometries/Sphere.h"
#include "Geometries/HittableList.h"
#include "Rendering/Camera.h"

int main() {
    // World (list of objects)
    HittableList world;

    auto material_ground = make_shared<LambertianMaterial>(
        Color(0.8, 0.8, 0.0)
    );
    auto material_red = make_shared<LambertianMaterial>(
            Color(0.7, 0.3, 0.3));
    auto material_blue = make_shared<LambertianMaterial>(
            Color(0.1, 0.3, 0.7));
    auto material_mirror = make_shared<MetalMaterial>(
            Color(0.8, 0.8, 0.8));
    auto material_gold = make_shared<MetalMaterial>(
            Color(0.8, 0.6, 0.3), 0.5);
    auto material_glass = make_shared<DielectricMaterial>(1.5);

    // helper lambda to add a sphere to the world
    auto add_sphere = 
        [&world](Point3 center, REAL radius, shared_ptr<Material> material){
            world.add(make_shared<Sphere>(center, radius, material));
    };

    add_sphere({ 0.0, -100.5, -1.0 }, 100, material_ground);
    // Interesting & easy trick with dielectric spheres:
    //  using negative radius, the geometry is unaffected, but the surface
    //  normal points inward. We can use this as a "bubble" to make a hollow
    //  glass sphere.
    //Left Sphere
    add_sphere({-1.0,    0.0, -1.0 }, 0.5, material_glass);
    add_sphere({-1.0,    0.0, -1.0 }, -0.4, material_glass);

    //Center Sphere
    add_sphere({ 0.0,    0.0, -1.0 }, 0.5, material_blue);

    //Right Sphere
    add_sphere({+1.0,    0.0, -1.0 }, 0.5, material_gold);

    // Image
    Camera camera;
    /* camera.camera_center        = Point3(0.0, 0.2, 0.2); */
    camera.aspect_ratio         = 16.0 / 9.0;
    camera.image_width          = 400;
    camera.samples_per_pixel    = 100;
    camera.max_depth            =  30;

    // Render
    camera.render(world);

    return 0;
}
