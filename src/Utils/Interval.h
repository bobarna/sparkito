#ifndef INTERVAL_H
#define INTERVAL_H

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
};

const static Interval EMPTY;
const static Interval EVERYTHING(-INFINITY, INFINITY);
const static Interval TO_INFINITY(0, INFINITY);


#endif //INTERVAL_H
