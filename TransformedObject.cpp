#include "TransformedObject.h"

TransformedObject::TransformedObject(const std::shared_ptr<Hittable>& obj, const Transform& t)
    : object(obj), transform(t)
{
    transformInv = Transform::Inverse(transform);
}

bool TransformedObject::Hit(const Ray& ray, HitRecord& rec) const
{
    auto hit = object->Hit(transformInv.TransformRay(ray), rec);
    if(hit)
    {
        rec.position = transform.TransformPoint(rec.position);
        rec.normal = Normalize(transform.TransformNormal(rec.normal));
        return true;
    }
    return false;
}

Vector3 TransformedObject::Sample(const Point3& o) const
{
    auto oo = object->Sample(transformInv.TransformPoint(o));
    auto vv = Normalize(transform.TransformVector(oo));
    return vv;
}

double TransformedObject::Pdf(const Point3& o, const Vector3& v) const
{
    auto oo = transformInv.TransformPoint(o);
    auto vv = Normalize(transformInv.TransformVector(v));
    return object->Pdf(oo, vv);
}