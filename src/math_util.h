
#include <math.h>
#include <stdbool.h>

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

struct Point {
    float x;
    float y;
};

struct Rectangle {
    // NOTE: should always have pos1 <= pos2
    struct Point pos1;
    struct Point pos2;
};

struct Circle {
    struct Point center;
    float radius;
};

struct Speed {
    float x;
    float y;
};

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};


inline float sqr(float x) { return x*x; }

float totalSpeed(struct Speed speed);
struct Speed setTotalSpeed(struct Speed speed, float total);
float speedAngle(struct Speed speed);

float distance(struct Point p1, struct Point p2);
float angleBetween(struct Point p1, struct Point p2);

bool overlapping(const struct Circle c1, const struct Circle c2);

float normAngle(float angle);


#endif /* end of include guard: MATH_UTIL_H */
