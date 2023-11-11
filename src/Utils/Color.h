#ifndef COLOR_H
#define COLOR_H

#include "../Sparkito.h"

#include <iostream>

using Color = Vec3;

// inverse of "gamma 2" model
inline REAL linear_to_gamma(REAL linear_component) {
    return sqrt(linear_component);
}

// inverse of "gamma 2" model
inline void linear_to_gamma(Color& linear_color) {
    for(int i = 0; i < 3; ++i)
        linear_color[i] = sqrt(linear_color[i]);
}

// Optionally multi-sampled color measurement for getting rid of "jaggies"
void write_color(std::ostream &out, 
                 Color pixel_color, 
                 int samples_per_pixel = 1) 
{
    // Divide the color by the number of samples
    pixel_color /= samples_per_pixel;

    // Apply the linear to gamma transform
    linear_to_gamma(pixel_color);

    // Write [0,255] color of each RGB component.
    static const Interval intensity(0.0000, 0.9999);
    intensity.clamp(pixel_color);
    
    out << static_cast<int>( 256 * pixel_color.r() ) << ' '
        << static_cast<int>( 256 * pixel_color.g() ) << ' '
        << static_cast<int>( 256 * pixel_color.b() ) << '\n';
}

#endif
