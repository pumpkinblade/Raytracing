#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Hittable.h"

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Point3 &p0, const Point3 &p1, const Point3 &p2, const std::shared_ptr<Material> &m);
    Triangle(const Point3 &p0, const Point3 &p1, const Point3 &p2, const Vector3 &n, const std::shared_ptr<Material> &m);

    virtual bool Hit(const Ray &ray, HitRecord &rec) const override;
    virtual Vector3 Sample(const Point3 &o) const override;
    virtual double Pdf(const Point3 &o, const Vector3 &v) const;
    virtual Bounds3 GetBounds() const override;

public:
    Point3 vertices[3];
    Vector3 normal;
    double area;
    std::shared_ptr<Material> material;
};

#endif