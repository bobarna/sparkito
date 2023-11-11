#ifndef CAMERA_H
#define CAMERA_H

#include "../Sparkito.h"
#include "../Utils/Color.h"
#include "../Geometries/Hittable.h"

#include <iostream>

/* 
 * The Camera class is responsible for
 *  - Storing the camera parameters
 *  - Creating the rays
 *  - Shooting out the rays for each pixel (i.e. rendering the image)
 * */

class Camera {
public:

    REAL aspect_ratio = 1.0; // = image_width / image_height
    int image_width = 100; // number of horizontal pixels
    Point3 camera_center = Point3(0,0,0);
    int samples_per_pixel = 10;

    void render(const HittableList& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int j = 0; j < image_height; ++j){
            std::clog << "\rScanlines remaining: " 
                      << (image_height - j) << " " << std::flush;
            for(int i = 0; i < image_width; ++i){
                // Sample multiple rays per pixel
                Color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray r = get_ray(i,j);
                    pixel_color += ray_color(r, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\rDone.\n";
    }

private:
    int image_height;
    REAL focal_length = 1.0;
    REAL viewport_height = 2.0;
    REAL viewport_width; 
    Point3 pixel00_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        // Make sure that image height is at least 1
        image_height = (image_height < 1) ? 1 : image_height;

        // Determine viewport dimensions
        viewport_width = viewport_height * 
            (static_cast<REAL>(image_width) / image_height);

        // Helper variables for handling the camera
        // vectors along the horizontal (u) and down the vertical (v) edges 
        Vec3 viewport_u = Vec3(viewport_width, 0               , 0);
        Vec3 viewport_v = Vec3(0             , -viewport_height, 0);
        // horizontal (u) and vertical (v) delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Location of the upper left pixel
        Vec3 viewport_upper_left = camera_center
                                    - Vec3(0, 0, focal_length)
                                    - viewport_u/2
                                    - viewport_v/2;
        // we measure each pixel at the center of the grid elements
        pixel00_loc = viewport_upper_left
                            + 0.5 * (pixel_delta_u + pixel_delta_v);

    }

    Ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location (i, j)
        Vec3 pixel_center = pixel00_loc 
                            + (i * pixel_delta_u)
                            + (j * pixel_delta_v);

        // get random ray in the pixel to be sampled
        Vec3 pixel_sample = pixel_sample_square(pixel_center);
        Vec3 ray_direction = pixel_sample - camera_center;

        return Ray(camera_center, ray_direction);
    }

    // TODO experimenting with sampling on a disk instead?
    Vec3 pixel_sample_square(const Vec3 pixel_center) const {
        // Returns a random point in the pixel surrounding pixel_center
        REAL px = -0.5 + random_real(); 
        REAL py = -0.5 + random_real(); 
        return pixel_center + (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    Color ray_color(const Ray& ray, const HittableList& world) const { 
        HitRecord hit;

        if(world.hit(ray, TO_INFINITY, hit)) {
            // we hit something -> render it
            Vec3 bounce_dir = get_random_on_hemisphere(hit.normal);
            // render normal color
            //return 0.5 * Color(hit.normal + Color(1,1,1)); // 0 < N < 1
            // simulate bounce
            REAL albedo = 0.5;
            return albedo * ray_color(Ray(hit.p, bounce_dir), world);
        }

        // render sky if we didn't hit any objects
        REAL y = unit_vector(ray.direction()).y(); // -1.0 < y < 1.0
        REAL a = 0.5 * (y + 1.0); // 0.0 < a < 1.0

        // linear blend between white and blue
        Color white(1.0, 1.0, 1.0);
        Color blue(0.5, 0.7, 1.0);
        return (1.0-a)*white + a*blue;
    }

};

#endif //CAMERA_H
