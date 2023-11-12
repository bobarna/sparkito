#ifndef CAMERA_H
#define CAMERA_H

#include "../Sparkito.h"
#include "../Utils/Color.h"
#include "../Geometries/Hittable.h"
#include "Material.h"

#include <iostream>

/* 
 * The Camera class is responsible for
 *  - Storing the camera parameters
 *  - Creating the rays
 *  - Shooting out the rays for each pixel (i.e. rendering the image)
 * */

class Camera {
public:
    REAL aspect_ratio       = 1.0; // = image_width / image_height
    int image_width         = 100; // number of horizontal pixels
    int samples_per_pixel   = 10; // Number of random samples for each pixel
    int max_depth           = 10; // Maximum number of ray bounces 

    REAL vfov = 90; // Vertical view angle (Field of View)                              
    Point3 camera_center    = Point3(0, 0, 0); // Point camera is looking from
    Point3 lookat           = Point3(0, 0, -1); // Point camera is looking at
    Point3 vup              = Vec3(0,1,0); // Camera-relative "up" direction 

    REAL defocus_angle = 10.0;  // Variation angle of rays through each pixel
    REAL focus_dist    = 3.4; // Distance from camera center to plane of perfect
                             // focus

    void render(const HittableList& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        
        #pragma omp parallel
        for(int j = 0; j < image_height; ++j){
            std::clog << "\rScanlines remaining: " 
                      << (image_height - j) << " " << std::flush;
            #pragma omp parallel
            for(int i = 0; i < image_width; ++i){
                // Sample multiple rays per pixel
                Color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\rDone.\n";
    }

private:
    int image_height;
    REAL viewport_height = 2.0;
    REAL viewport_width; 
    Point3 pixel00_loc; // Location of (center of) pixel 0,0 
    Vec3 pixel_delta_u; // Offset to pixel to the right (and left)
    Vec3 pixel_delta_v; // Offset to the pixel below (and above)
    Vec3 u, v, w; // Camera frame basis vectors
    Vec3 defocus_disk_u; // Defocus disk horizontal radius
    Vec3 defocus_disk_v; // Defocus disk vertical radius 

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        // Make sure that image height is at least 1
        image_height = (image_height < 1) ? 1 : image_height;

        // Determine viewport dimensions
        REAL theta = degrees_to_radians(vfov);
        REAL h = tan(theta/2);
        viewport_height = 2 * h * focus_dist;
        viewport_width = viewport_height * (REAL(image_width) / image_height);

        // Calculate {u,v,w} unit basis vectors for the camera coordinate frame
        w = unit_vector(camera_center - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // vectors along the horizontal (u) and down the vertical (v) edges 
        Vec3 viewport_u = viewport_width * u;  // Vector across viewport
                                               // horizontal edge
        Vec3 viewport_v = viewport_height * -v; // Vector down viewport
                                                 // vertical edge

        // horizontal (u) and vertical (v) delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Location of the upper left pixel
        Point3 viewport_upper_left = camera_center
                                    - (focus_dist * w)
                                    - viewport_u/2
                                    - viewport_v/2;
        pixel00_loc = viewport_upper_left
                            + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        REAL defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    Ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location (i, j),
        // originating from the camera defocus disk.
        Vec3 pixel_center = pixel00_loc 
                            + (i * pixel_delta_u)
                            + (j * pixel_delta_v);
        Vec3 pixel_sample = pixel_sample_square(pixel_center);

        // rays originate from the camera center if there is no defocus. Else,
        // a random position is sampled from the defocus disk.
        Point3 ray_origin = 
            (defocus_angle <= 0 ) ? camera_center : defocus_disk_sample();
        Vec3 ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Point3 pixel_sample_square(const Vec3 pixel_center) const {
        // Returns a random point in the pixel surrounding pixel_center
        REAL px = -0.5 + random_real(); 
        REAL py = -0.5 + random_real(); 
        return pixel_center + (px * pixel_delta_u) + (py * pixel_delta_v);
    }
    
    Point3 defocus_disk_sample() const {
        // Returns a random point in the camera focus disk.
        Point3 p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    Color ray_color(const Ray& ray, 
                    int depth, 
                    const HittableList& world) const { 
        HitRecord hit;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if(depth <= 0) 
            return Color(0,0,0);

        if(world.hit(ray, EPS_TO_INFINITY, hit)) {
            // we hit something -> render it
            // Uniform sampling:
            // Vec3 bounce_dir = get_random_on_hemisphere(hit.normal);
            // True lambertian shading instead of simple hemisphere.
            // (Scattering is more probable towards the surface normal)
            // Vec3 bounce_dir = hit.normal + get_random_unit_vector();

            Ray scattered;
            Color attenuation;
            if(hit.material->scatter(ray, hit, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);

            return Color(0,0,0);
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
