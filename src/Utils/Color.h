#ifndef COLOR_H
#define COLOR_H

#include "../Math/Vec3.h"

#include <iostream>

using Color = Vec3;

void write_color(std::ostream &out, Color pixel_color) {
    // Write [0,255] color of each RGB component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif
