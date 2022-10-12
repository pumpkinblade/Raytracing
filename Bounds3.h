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
    Point3 Diagonal() const { return maximum - minimum; }
    Point3 Centeriod() const { return 0.5 * (minimum + maximum); }
    double Area() const
    {
        Vector3 diag = Diagonal();
        return 2 * (diag.X() * diag.Y() + diag.Y() * diag.Z() + diag.Z() * diag.X());
    }

    bool HitP(const Ray &ray, const Vector3 &dirInv, const Vector3 &dirIsNeg) const
    {
        Vector3 t_min_axis = dirInv * (maximum * dirIsNeg + minimum * (Vector3(1.0) - dirIsNeg) - ray.origin);
        Vector3 t_max_axis = dirInv * (minimum * dirIsNeg + maximum * (Vector3(1.0) - dirIsNeg) - ray.origin);
        double t_min = std::max(std::max(t_min_axis(0), t_min_axis(1)), t_min_axis(2));
        double t_max = std::min(std::min(t_max_axis(0), t_max_axis(1)), t_max_axis(2));
        return t_min <= t_max && t_max > 0.0;
    }

    static Bounds3 Union(const Bounds3 &b1, const Bounds3 &b2)
    {
        Point3 pMin = Point3(std::min(b1.Min().X(), b2.Min().X()),
                             std::min(b1.Min().Y(), b2.Min().Y()),
                             std::min(b1.Min().Z(), b2.Min().Z()));
        Point3 pMax = Point3(std::max(b1.Max().X(), b2.Max().X()),
                             std::max(b1.Max().Y(), b2.Max().Y()),
                             std::max(b1.Max().Z(), b2.Max().Z()));
        return Bounds3(pMin, pMax);
    }

private:
    Point3 minimum, maximum;
};

#endif