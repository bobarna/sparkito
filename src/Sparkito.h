#ifndef SPARKITO_H
#define SPARKITO_H

#include <cmath>
#include <limits>
#include <memory>

#include <iostream>

// Usings 
using REAL = double;

using std::sqrt;
using std::shared_ptr;
using std::make_shared;

// Constants

#undef INFINITY //comes with <cmath>
const REAL INFINITY = std::numeric_limits<REAL>::infinity();
const REAL PI = 3.1415926535897932385;

// Utility functions

inline REAL degrees_to_radians(REAL degrees) {
    return degrees * PI / 180.0;
}

// Common Headers
#include "Math/Vec3.h"
#include "Geometries/Ray.h"

#endif // SPARKITO_H
