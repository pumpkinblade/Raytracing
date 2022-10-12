#include "TransformedObject.h"

TransformedObject::TransformedObject(const std::shared_ptr<Hittable> &obj, const Transform &t)
    : object(obj), transform(t)
{
    transformInv = Transform::Inverse(transform);
    Point3 ps[2] = {object->GetBounds().Min(), object->GetBounds().Max()};
    Point3 pMin = Point3(FT_MAX);
    Point3 pMax = Point3(FT_MIN);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                Point3 tmp = Point3(ps[i].X(), ps[j].Y(), ps[k].Z());
                tmp = transform.TransformPoint(tmp);
                pMin = Point3(std::min(tmp.X(), pMin.X()), std::min(tmp.Y(), pMin.Y()), std::min(tmp.Z(), pMin.Z()));
                pMax = Point3(std::max(tmp.X(), pMax.X()), std::max(tmp.Y(), pMax.Y()), std::max(tmp.Z(), pMax.Z()));
            }
        }
    }
    bounds = Bounds3(pMin, pMax);
}

bool TransformedObject::Hit(const Ray &ray, HitRecord &rec) const
{
    auto hit = object->Hit(transformInv.TransformRay(ray), rec);
    if (hit)
    {
        rec.position = transform.TransformPoint(rec.position);
        rec.normal = Normalize(transform.TransformNormal(rec.normal));
        return true;
    }
    return false;
}

Vector3 TransformedObject::Sample(const Point3 &o) const
{
    auto oo = object->Sample(transformInv.TransformPoint(o));
    auto vv = Normalize(transform.TransformVector(oo));
    return vv;
}

double TransformedObject::Pdf(const Point3 &o, const Vector3 &v) const
{
    auto oo = transformInv.TransformPoint(o);
    auto vv = Normalize(transformInv.TransformVector(v));
    return object->Pdf(oo, vv);
}

Bounds3 TransformedObject::GetBounds() const
{
    return bounds;
}