
#include <math.h>
#include <stdbool.h>

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

struct Vector {
    float x;
    float y;
};

typedef struct Vector Point;
typedef struct Vector Speed;

struct Rectangle {
    // NOTE: should always have pos1 <= pos2
    Point pos1;
    Point pos2;
};

struct Circle {
    Point center;
    float radius;
};

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};


inline float sqr(float x) { return x*x; }

struct Vector addVec(struct Vector v1, struct Vector v2);
struct Vector subVec(struct Vector v1, struct Vector v2);
struct Vector multScalar(float scalar, struct Vector vec);
float dotVec(struct Vector v1, struct Vector v2);

float magnitude(struct Vector vec);
struct Vector setMagnitude(struct Vector vec, float magn);
float projectMultiplier(struct Vector vec, struct Vector onto);
struct Vector projectOnto(struct Vector vec, struct Vector onto);
float angleVec(struct Vector vec);

float distance(struct Vector v1, struct Vector v2);
float angleBetween(struct Vector v1, struct Vector v2);

bool overlapping(const struct Circle c1, const struct Circle c2);

float normAngle(float angle);


#endif /* end of include guard: MATH_UTIL_H */
