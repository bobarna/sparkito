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
    // helper lambda to add a sphere to the world
    auto add_sphere = 
        [&world](Point3 center, REAL radius, shared_ptr<Material> material){
            world.add(make_shared<Sphere>(center, radius, material));
    };

    auto material_ground = make_shared<LambertianMaterial>(Color(0.55, 0.55, 0.2));
    add_sphere({ 0.0, -1500, 0.0 }, 1500, material_ground);

    //auto material_gold = make_shared<MetalMaterial>( Color(0.8, 0.6, 0.3), 0.5);
    auto material_blue = make_shared<LambertianMaterial>( Color(0.3, 0.3, 0.6));
    auto material_glass = make_shared<DielectricMaterial>(1.5);
    auto material_mirror = make_shared<MetalMaterial>(Color(0.8, 0.8, 0.8));
    REAL small_radius = 0.2;
    Point3 big_center_left( -4.0, 1.0, 1.0 );
    Point3 big_center_center( 0.0, 1.0, 0.2 );
    Point3 big_center_right( 4.0, 1.0, 0 );
    for(int x = -11; x < 11; ++x) {
        for(int z = -11; z < 11; ++z) {
            // Random position
            Point3 curr_center(
                x + (1-small_radius/2)*random_real(), 
                small_radius,
                z + (1-small_radius/2)*random_real()
            );
            if(
                (curr_center - big_center_left).length() < 1 + small_radius
                || (curr_center - big_center_center).length() < 1 + small_radius
                || (curr_center - big_center_right).length() < 1 + small_radius
            )
                // Don't generate small spheres inside the big spheres
                continue;

            auto choose_mat = random_real(); // in [0,1)
            shared_ptr<Material> curr_material;
            // Random Material
            if(choose_mat < 0.8) {
                // Diffuse Material
                Color curr_albedo = Color::get_random() * Color::get_random();
                curr_material = make_shared<LambertianMaterial>(curr_albedo);
            } else if (choose_mat < 0.95) {
                // Metal Material
                Color curr_albedo = Color::get_random(0.5, 1);
                REAL curr_fuzz = random_real(0, 0.5); 
                curr_material = make_shared<MetalMaterial>(curr_albedo, curr_fuzz);
            } else {
                // Glass Material
                curr_material = material_glass;
            }
            // Add random small sphere
            add_sphere(curr_center, small_radius, curr_material);
        }
    }

    add_sphere(big_center_left,   1.0, material_blue); // left
    add_sphere(big_center_center, 1.0, material_glass); // center
    add_sphere(big_center_right,  1.0, material_mirror); // right
    // Interesting & easy trick with dielectric spheres: using negative radius,
    // the geometry is unaffected, but the surface normal points inward. We can
    // use this as a "bubble" to make a hollow glass sphere.
    // add_sphere({0.0, 0.4, -1.0 }, -0.4, material_glass); // center (inner)

    // Image
    Camera camera;
    camera.aspect_ratio         = 16.0 / 9.0;
    camera.image_width          = 1200;
    camera.samples_per_pixel    = 500;
    camera.max_depth            = 50;

    camera.vfov             = 20;
    //camera.camera_center        = Point3(0.0, 0.2, 0.2);
    camera.camera_center    = Point3(13, 2, 3);
    camera.lookat           = Point3(0,0,0);
    camera.vup              = Vec3(0,1,0);

    // defocus blur (i.e. depth of field)
    camera.defocus_angle = 0.6;
    camera.focus_dist = 10.0;

    // Render
    camera.render(world);

    return 0;
}
