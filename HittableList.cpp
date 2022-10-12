#include "HittableList.h"

void HittableList::Add(const std::shared_ptr<Hittable> &obj)
{
    objects.push_back(obj);
    bounds = Bounds3::Union(obj->GetBounds(), bounds);
}

bool HittableList::Hit(const Ray &ray, HitRecord &rec) const
{
    HitRecord tmp;
    bool hit = false;
    double closest = INFTY;

    for (const auto &obj : objects)
    {
        if (obj->Hit(ray, tmp))
        {
            if (tmp.t < closest)
            {
                closest = tmp.t;
                rec = tmp;
                hit = true;
            }
        }
    }
    return hit;
}

Vector3 HittableList::Sample(const Point3 &o) const
{
    int idx = RandomInt(0, objects.size());
    return objects[idx]->Sample(o);
}

double HittableList::Pdf(const Point3 &o, const Vector3 &v) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->Pdf(o, v);

    return sum;
}

Bounds3 HittableList::GetBounds() const
{
    return bounds;
}