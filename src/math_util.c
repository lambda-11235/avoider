
#include "math_util.h"

struct Vector addVec(struct Vector v1, struct Vector v2) {
    return (struct Vector) {v1.x + v2.x, v1.y + v2.y};
}


struct Vector subVec(struct Vector v1, struct Vector v2) {
    return (struct Vector) {v1.x - v2.x, v1.y - v2.y};
}


struct Vector multScalar(float scalar, struct Vector vec) {
    return (struct Vector) {scalar*vec.x, scalar*vec.y};
}


float dotVec(struct Vector v1, struct Vector v2) {
    return v1.x*v2.x + v1.y*v2.y;
}


float magnitude(struct Vector vec) {
    return sqrt(sqr(vec.x) + sqr(vec.y));
}


struct Vector setMagnitude(struct Vector vec, float magn) {
    float magnOld = magnitude(vec);

    if (magnOld > 0)
        vec = multScalar(magn/magnOld, vec);

    return vec;
}


float projectMultiplier(struct Vector vec, struct Vector onto) {
    return dotVec(vec, onto)/dotVec(onto, onto);
}


struct Vector projectOnto(struct Vector vec, struct Vector onto) {
    return multScalar(projectMultiplier(vec, onto), onto);
}


float angleVec(struct Vector vec) {
    float angle = atan(vec.y/vec.x);

    if (vec.x < 0)
        angle = M_PI + angle;

    return angle;
}


float distance(struct Vector v1, struct Vector v2) {
    return magnitude(subVec(v1, v2));
}


float angleBetween(struct Vector v1, struct Vector v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float angle = atan(dy/dx);

    if (dx < 0)
        angle = M_PI + angle;

    return angle;
}


bool overlapping(const struct Circle c1, const struct Circle c2) {
    return distance(c1.center, c2.center) < c1.radius + c2.radius;
}


float normAngle(float angle) {
    while (angle > 2*M_PI)
        angle -= 2*M_PI;

    while (angle < 0)
        angle += 2*M_PI;

    return angle;
}
