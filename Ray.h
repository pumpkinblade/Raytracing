#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

struct Ray
{
    Ray() = default;
    Ray(const Point3& o, const Vector3& dir) : origin(o), direction(dir) {}

    Point3 At(double t) const { return origin + t*direction; }

    Point3 origin;
    Vector3 direction;
};

#endif