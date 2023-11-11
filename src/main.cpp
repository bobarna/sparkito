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
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    // Image
    Camera camera;
    camera.aspect_ratio         = 16.0 / 9.0;
    camera.image_width          = 400;
    camera.samples_per_pixel    = 200;
    camera.max_depth            =  50;

    // Render
    camera.render(world);

    return 0;
}
