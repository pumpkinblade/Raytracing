#ifndef BVH_H
#define BVH_H

#include "rtbase.h"
#include "Hittable.h"
#include "Bounds3.h"
#include <vector>

struct BvhNode
{
    Bounds3 bounds;
    std::shared_ptr<BvhNode> left;
    std::shared_ptr<BvhNode> right;
    std::shared_ptr<Hittable> object;

    BvhNode() : bounds(), left(nullptr), right(nullptr), object(nullptr) {}
    BvhNode(std::vector<std::shared_ptr<Hittable>> objs);
    bool Hit(const Ray &ray, const Vector3 &dirInv, const Vector3 &dirIsNeg, HitRecord &rec) const;
};

class Bvh : public Hittable
{
public:
    Bvh() = default;
    Bvh(const std::vector<std::shared_ptr<Hittable>> &objs);

    virtual bool Hit(const Ray &ray, HitRecord &rec) const override;
    virtual Vector3 Sample(const Point3 &o) const override;
    virtual double Pdf(const Point3 &o, const Vector3 &v) const override;
    virtual Bounds3 GetBounds() const override;

public:
    std::vector<std::shared_ptr<Hittable>> objects;
    std::shared_ptr<BvhNode> root;
};

#endif