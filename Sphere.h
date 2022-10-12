#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "rtbase.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(const Point3 &c, double r, const std::shared_ptr<Material> &m);

    virtual bool Hit(const Ray &ray, HitRecord &rec) const override;
    virtual Vector3 Sample(const Point3 &o) const override;
    virtual double Pdf(const Point3 &o, const Vector3 &v) const;
    virtual Bounds3 GetBounds() const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};

#endif