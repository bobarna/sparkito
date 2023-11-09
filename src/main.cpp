#include <iostream>

#include "Sparkito.h"
#include "Math/Vec3.h"
#include "Utils/Color.h"
#include "Geometries/Ray.h"
#include "Geometries/Sphere.h"
#include "Geometries/HittableList.h"

Color ray_color(const Ray& ray, const HittableList& world) {
    HitRecord hit;
    if(world.hit(ray, 0, INFINITY, hit)) {
        // we hit something -> render it
        Vec3 N = hit.normal;
        return 0.5 * Color(N.x()+1, N.y()+1, N.z()+1); // 0 < N < 1
    }

    // render sky if we didn't hit any objects
    REAL y = unit_vector(ray.direction()).y(); // -1.0 < y < 1.0
    REAL a = 0.5 * (y + 1.0); // 0.0 < a < 1.0

    Color white(1.0, 1.0, 1.0);
    Color blue(0.5, 0.7, 1.0);
    // linear blend between white and blue
    return (1.0-a)*white + a*blue;
}

int main() {
    // Image
    REAL aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    // Make sure that image height is at least 1
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    // In 3D (virtual) space; < 1 values are OK as they are real-valued
    REAL focal_length = 1.0;
    REAL viewport_height = 2.0;
    REAL viewport_width = viewport_height * 
        (static_cast<REAL>(image_width) / image_height);
    Point3 camera_center = Point3(0,0,0);

    // World (list of objects)
    Point3 sphere_center = Point3(0,0,-1);
    REAL sphere_radius = 0.5;
    HittableList world(make_shared<Sphere>(sphere_center, sphere_radius));

    // Helper variables for handling the camera
    // vectors along the horizontal (u) and down the vertical (v) edges 
    Vec3 viewport_u = Vec3(viewport_width, 0               , 0);
    Vec3 viewport_v = Vec3(0             , -viewport_height, 0);
    // horizontal (u) and vertical (v) delta vectors from pixel to pixel
    Vec3 pixel_delta_u = viewport_u / image_width;
    Vec3 pixel_delta_v = viewport_v / image_height;

    // Location of the upper left pixel
    Vec3 viewport_upper_left = camera_center
                                - Vec3(0, 0, focal_length)
                                - viewport_u/2
                                - viewport_v/2;
    // we measure each pixel at the center of the grid elements
    Vec3 pixel00_loc = viewport_upper_left
                        + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; ++j){
        std::clog << "\rScanlines remaining: " 
                  << (image_height - j) << " " << std::flush;
        for(int i = 0; i < image_width; ++i){
            Vec3 pixel_center = pixel00_loc 
                                + (i * pixel_delta_u)
                                + (j * pixel_delta_v);
            Vec3 ray_direction = pixel_center - camera_center;
            Ray curr_ray(camera_center, ray_direction);
            
            Color pixel_color = ray_color(curr_ray, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.\n";

    return 0;
}
