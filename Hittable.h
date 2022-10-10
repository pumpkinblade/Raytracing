#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <vector>
#include "rtbase.h"
#include "Material.h"

struct HitRecord
{
    Point3 position;
    Vector3 normal;
    double t;
    std::shared_ptr<Material> material;
};

class Hittable
{
public:
    virtual bool Hit(const Ray& ray, HitRecord& rec) const = 0;
    virtual Vector3 Sample(const Point3& o) const = 0;
    virtual double Pdf(const Point3& o, const Vector3& v) const = 0;
    // virtual Bounds3 GetBounds() const = 0;
};
#endif