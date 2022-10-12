#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtbase.h"
#include "Transform.h"

class Material
{
public:
    virtual Color Emit(const Point3 &p, const Vector3 &n, const Vector3 &wi) const { return Color(0.0, 0.0, 0.0); }
    virtual bool Scatter(const Point3 &p, const Vector3 &n, const Vector3 &wi, Vector3 &wo) const { return false; }
    virtual double Pdf(const Point3 &p, const Vector3 &n, const Vector3 &wi, const Vector3 &wo) const { return 0.0; }
    virtual Color Brdf(const Point3 &p, const Vector3 &n, const Vector3 &wi, const Vector3 &wo) const { return Color(0.0, 0.0, 0.0); }
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Color &e) : emission(e) {}

    virtual Color Emit(const Point3 &p, const Vector3 &n, const Vector3 &wi) const override
    {
        return Dot(n, wi) > 0 ? emission : Vector3(0.0, 0.0, 0.0);
    }

private:
    Color emission;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color &a) : albedo(a) {}

    virtual bool Scatter(const Point3 &p, const Vector3 &n, const Vector3 &wi, Vector3 &wo) const override
    {
        auto t = Transform::FromNormal(n);
        wo = t.TransformVector(RandomInHemiSphereCos());
        return true;
    }

    virtual double Pdf(const Point3 &p, const Vector3 &n, const Vector3 &wi, const Vector3 &wo) const override
    {
        return std::max(Dot(n, wo), 0.0) / PI;
    }

    virtual Color Brdf(const Point3 &p, const Vector3 &n, const Vector3 &wi, const Vector3 &wo) const override
    {
        return albedo / PI;
    }

private:
    Color albedo;
};

#endif