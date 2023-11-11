#ifndef SPARKITO_H
#define SPARKITO_H

#include <cmath>
#include <cstdlib> // for rand
#include <time.h> // time() for rand seed
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
const REAL EPS = 0.001;

// Utility functions

inline REAL degrees_to_radians(REAL degrees) {
    return degrees * PI / 180.0;
}

// -- Random numbers

inline void set_random_seed() {
    srand(time(NULL));
}

inline void set_random_seed(unsigned int seed) {
    srand(seed);
}

inline REAL random_real() {
    // returns a random real in [0.0; 1.0)
    return REAL( rand() / (RAND_MAX + 1.0) );
}

inline REAL random_real(REAL min, REAL max) {
    // returns a random real in [min, max)
    return REAL( min + (max-min)*random_real() );
}

// Common Headers
#include "Utils/Interval.h"
#include "Math/Vec3.h"
#include "Geometries/Ray.h"

#endif // SPARKITO_H
