#ifndef TRANSFORMED_OBJECT_H
#define TRANSFORMED_OBJECT_H

#include "Hittable.h"
#include "Transform.h"
#include <memory>

class TransformedObject : public Hittable
{
public:
    TransformedObject() = default;
    TransformedObject(const std::shared_ptr<Hittable> &obj, const Transform &t);

    virtual bool Hit(const Ray &ray, HitRecord &rec) const override;
    virtual Vector3 Sample(const Point3 &o) const override;
    virtual double Pdf(const Point3 &o, const Vector3 &v) const;
    virtual Bounds3 GetBounds() const override;

private:
    std::shared_ptr<Hittable> object;
    Transform transform;
    Transform transformInv;
    Bounds3 bounds;
};

#endif