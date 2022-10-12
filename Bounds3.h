#ifndef BOUNDS_3_H
#define BOUNDS_3_H

#include "rtbase.h"

class Bounds3
{
public:
    Bounds3() : minimum(FT_MAX), maximum(FT_MIN) {}
    Bounds3(const Point3 &min, const Point3 &max) : minimum(min), maximum(max) {}

    const Point3 &Min() const { return minimum; }
    const Point3 &Max() const { return maximum; }

    bool HitP(const Ray &ray, const Vector3 &dirInv, const Vector3 &dirIsNeg) const;

    static Bounds3 Union(const Bounds3 &b1, const Bounds3 &b2);

private:
    Point3 minimum, maximum;
};

#endif