#include "Triangle.h"

Triangle::Triangle(const Point3 &p0, const Point3 &p1, const Point3 &p2, const std::shared_ptr<Material> &m)
    : vertices{p0, p1, p2}, material(m)
{
    auto n = Cross(p1 - p0, p2 - p0);
    normal = Normalize(n);
    area = Length(n) * 0.5;
}

Triangle::Triangle(const Point3 &p0, const Point3 &p1, const Point3 &p2, const Vector3 &n, const std::shared_ptr<Material> &m)
    : vertices{p0, p1, p2}, material(m)
{
    normal = Normalize(n);
    area = Length(Cross(p1 - p0, p2 - p0)) * 0.5;
}

bool Triangle::Hit(const Ray &ray, HitRecord &rec) const
{
    auto e1 = vertices[1] - vertices[0];
    auto e2 = vertices[2] - vertices[0];
    auto s = ray.origin - vertices[0];
    auto s1 = Cross(ray.direction, e2);
    auto s2 = Cross(s, e1);
    auto det = Dot(s1, e1);

    auto t = Dot(s2, e2) / det;
    auto b1 = Dot(s1, s) / det;
    auto b2 = Dot(s2, ray.direction) / det;
    if (t <= 0 || b1 < 0 || b2 < 0 || b1 + b2 > 1)
        return false;

    rec.t = t;
    rec.position = ray.At(rec.t);
    rec.normal = normal;
    rec.material = material;
    return true;
}

Vector3 Triangle::Sample(const Point3 &o) const
{
    auto u = 1 - std::sqrt(1 - RandomFloat(0, 1));
    auto v = RandomFloat(0, 1 - u);
    auto p = (1 - u - v) * vertices[0] + u * vertices[1] + v * vertices[2];
    return Normalize(p - o);
}

double Triangle::Pdf(const Point3 &o, const Vector3 &v) const
{
    HitRecord rec;
    Ray ray(o + v * DELTA, v);
    if (!Hit(ray, rec))
        return 0.0;
    auto distSquared = rec.t * rec.t;
    auto cosine = std::abs(Dot(rec.normal, v));
    return distSquared / (cosine * area);
}