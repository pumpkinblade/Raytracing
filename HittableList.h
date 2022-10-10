#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
    HittableList() = default;

    void Add(const std::shared_ptr<Hittable>& obj);

    virtual bool Hit(const Ray& ray, HitRecord& rec) const override;
    virtual Vector3 Sample(const Point3& o) const override;
    virtual double Pdf(const Point3& o, const Vector3& v) const override;
    // virtual Bounds3 GetBounds() const override;

private:
    std::vector<std::shared_ptr<Hittable>> objs;
    // Bounds3 bounds;
};

#endif