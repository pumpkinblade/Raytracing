#include "Bvh.h"
#include <algorithm>

BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>> objs)
{
    for (const auto &o : objs)
    {
        bounds = Bounds3::Union(bounds, o->GetBounds());
    }

    if (objs.size() == 1)
    {
        object = objs[0];
        left = right = nullptr;
    }
    else if (objs.size() == 2)
    {
        left = std::make_shared<BvhNode>(std::vector{objs[0]});
        right = std::make_shared<BvhNode>(std::vector{objs[1]});
        object = nullptr;
    }
    else
    {
        // 选取整个包围盒的最长轴
        Vector3 diag = bounds.Max() - bounds.Min();
        int axis;
        if (diag.X() > diag.Y())
        {
            if (diag.X() > diag.Z())
                axis = 0;
            else
                axis = 2;
        }
        else
        {
            if (diag.Y() > diag.Z())
                axis = 1;
            else
                axis = 2;
        }
        auto comp = [axis](const std::shared_ptr<Hittable> &b1, const std::shared_ptr<Hittable> &b2)
        {
            double c1 = b1->GetBounds().Min()(axis) + b1->GetBounds().Max()(axis);
            double c2 = b2->GetBounds().Min()(axis) + b2->GetBounds().Max()(axis);
            return c1 < c2;
        };
        std::sort(objs.begin(), objs.end(), comp);
        auto mid = objs.size() / 2;
        left = std::make_shared<BvhNode>(std::vector(objs.begin(), objs.begin() + mid));
        right = std::make_shared<BvhNode>(std::vector(objs.begin() + mid, objs.end()));
    }
}

bool BvhNode::Hit(const Ray &ray, const Vector3 &dirInv, const Vector3 &dirIsNeg, HitRecord &rec) const
{
    if (!bounds.HitP(ray, dirInv, dirIsNeg))
        return false;
    if (left != nullptr && right != nullptr)
    {
        HitRecord rec_left;
        bool hit_left = left->Hit(ray, dirInv, dirIsNeg, rec_left);
        HitRecord rec_right;
        bool hit_right = right->Hit(ray, dirInv, dirIsNeg, rec_right);
        if (hit_left && hit_right)
        {
            if (rec_left.t < rec_right.t)
                rec = rec_left;
            else
                rec = rec_right;
            return true;
        }
        else if (hit_left)
        {
            rec = rec_left;
            return true;
        }
        else if (hit_right)
        {
            rec = rec_right;
            return true;
        }
        return false;
    }
    else
    {
        if (object != nullptr)
            return object->Hit(ray, rec);
        else
            return false;
    }
}

Bvh::Bvh(const std::vector<std::shared_ptr<Hittable>> &objs)
{
    objects = objs;
    root = std::make_shared<BvhNode>(objects);
}

bool Bvh::Hit(const Ray &ray, HitRecord &rec) const
{
    if (root == nullptr)
        return false;
    Vector3 dirInv = Vector3(1.0 / ray.direction.X(), 1.0 / ray.direction.Y(), 1.0 / ray.direction.Z());
    Vector3 dirIsNeg = Vector3(ray.direction.X() < 0, ray.direction.Y() < 0, ray.direction.Z() < 0);
    return root->Hit(ray, dirInv, dirIsNeg, rec);
}

Vector3 Bvh::Sample(const Point3 &o) const
{
    int idx = RandomInt(0, objects.size());
    return objects[idx]->Sample(o);
}

double Bvh::Pdf(const Point3 &o, const Vector3 &v) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->Pdf(o, v);

    return sum;
}

Bounds3 Bvh::GetBounds() const
{
    return root->bounds;
}
