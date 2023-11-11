#ifndef COLOR_H
#define COLOR_H

#include "../Math/Vec3.h"

#include <iostream>

using Color = Vec3;

// Optionally multi-sampled color measurement for getting rid of "jaggies"
void write_color(std::ostream &out, 
                 Color pixel_color, 
                 int samples_per_pixel = 1) 
{
    pixel_color /= samples_per_pixel;

    // Write [0,255] color of each RGB component.
    static const Interval intensity(0.0000, 0.9999);
    intensity.clamp(pixel_color);
    
    out << static_cast<int>( 256 * pixel_color.r() ) << ' '
        << static_cast<int>( 256 * pixel_color.g() ) << ' '
        << static_cast<int>( 256 * pixel_color.b() ) << '\n';
}

#endif
