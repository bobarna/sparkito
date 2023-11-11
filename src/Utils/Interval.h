#ifndef INTERVAL_H
#define INTERVAL_H

#include "../Math/Vec3.h"
#include "../Sparkito.h"

struct Interval {
    REAL min;
    REAL max;

    Interval(): min(+INFINITY), max(-INFINITY) {} // default interval is empty
    Interval(REAL _min, REAL _max): min(_min), max(_max) {}

    bool contains(REAL x) {
        return min <= x && x <= max;
    }

    bool surrounds(REAL x) {
        return min < x && x < max;
    }

    REAL clamp(REAL x) const {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    Vec3& clamp(Vec3& v) const {
        for(int i = 0; i < 3; ++i) {
            if(v[i] < min) v[i] = min;
            if(v[i] > max) v[i] = max;
        }

        return v;
    }

};

const static Interval EMPTY;
const static Interval EVERYTHING(-INFINITY, INFINITY);
const static Interval TO_INFINITY(0, INFINITY);
const static Interval EPS_TO_INFINITY(EPS, INFINITY);


#endif //INTERVAL_H
