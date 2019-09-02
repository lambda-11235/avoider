
#include "math_util.h"


float totalSpeed(struct Speed speed) {
    return sqrt(sqr(speed.x) + sqr(speed.y));
}


struct Speed setTotalSpeed(struct Speed speed, float total) {
    float totalOld = totalSpeed(speed);

    if (totalOld > 0) {
        speed.x *= total/totalOld;
        speed.y *= total/totalOld;
    }

    return speed;
}


float speedAngle(struct Speed speed) {
    float angle = atan(speed.y/speed.x);

    if (speed.x < 0)
        angle = M_PI + angle;

    return angle;
}


float distance(struct Point p1, struct Point p2) {
    return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}


float angleBetween(struct Point p1, struct Point p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
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
