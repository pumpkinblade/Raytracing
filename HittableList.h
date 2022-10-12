#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"
#include "Bounds3.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
    HittableList() = default;

    void Add(const std::shared_ptr<Hittable> &obj);
    const auto &Objects() const { return objects; }

    virtual bool Hit(const Ray &ray, HitRecord &rec) const override;
    virtual Vector3 Sample(const Point3 &o) const override;
    virtual double Pdf(const Point3 &o, const Vector3 &v) const override;
    virtual Bounds3 GetBounds() const override;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
    Bounds3 bounds;
};

#endif