#include "Sphere.h"
#include "Transform.h"

Sphere::Sphere(const Point3& c, double r, const std::shared_ptr<Material>& m)
    : center(c), radius(r), material(m)
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& rec) const
{
    Vector3 oc = ray.origin - center;
    double a = Dot(ray.direction, ray.direction);
    double half_b = Dot(oc, ray.direction);
    double c = Dot(oc, oc) - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if(discriminant < 0)
        return false;
    double sqrtd = std::sqrt(discriminant);
    // 取最小正实根
	double root = (-half_b - sqrtd) / a;
	if (root < 0.0)
	{
		root = (-half_b + sqrtd) / a;
		if (root < 0.0)
			return false;
	}

    rec.t = root;
    rec.position = ray.At(rec.t);
    rec.normal = (rec.position - center) / radius;
    rec.material = material;
    return true;
}

Vector3 Sphere::Sample(const Point3& o) const
{
	auto direction = center - o;
    auto dirZ = RandomToSphere(radius, LengthSquared(direction));
    auto transform = Transform::FromNormal(direction);
    return transform.TransformVector(dirZ);
}

double Sphere::Pdf(const Point3& o, const Vector3& v) const
{
	HitRecord record;
	if (!Hit(Ray(o + DELTA*v, v), record))
		return 0;

	auto cos_theta_max = std::sqrt(1. - radius * radius / LengthSquared(center - o));
	return 1 / (2 * PI * (1 - cos_theta_max));
}

// Bounds3 Sphere::GetBounds() const
// {
//     return Bounds3(
//         center - Vector3(radius),
//         center + Vector3(radius)
//     );
// }
