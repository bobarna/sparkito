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
    add_sphere({-1.0,    0.0, -1.0 },  0.5, material_glass); // left
    add_sphere({-1.0,    0.0, -1.0 }, -0.4, material_glass); // left (inner)
    add_sphere({ 0.0,    0.0, -1.0 },  0.5, material_blue); // center
    add_sphere({+1.0,    0.0, -1.0 },  0.5, material_gold); // right

    // Image
    Camera camera;
    camera.aspect_ratio         = 16.0 / 9.0;
    camera.image_width          = 400;
    camera.samples_per_pixel    = 200;
    camera.max_depth            =  30;

    camera.vfov             = 20;
    //camera.camera_center        = Point3(0.0, 0.2, 0.2);
    camera.camera_center    = Point3(-2, 2, 1);
    camera.lookat           = Point3(0,0,-1);
    camera.vup              = Vec3(0,1,0);

    // defocus blur (i.e. depth of field)
    camera.defocus_angle = 10.0;
    camera.focus_dist = 3.4;

    // Render
    camera.render(world);

    return 0;
}
