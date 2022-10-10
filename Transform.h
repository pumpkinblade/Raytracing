#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "rtbase.h"
#include "Matrix3.h"

class Transform
{
public:
    Transform();
    Transform(const Matrix3 &mm, const Vector3 &vv);
    Transform(const Point3 &o, const Point3 &uu, const Point3 &vv, const Point3 &ww);

    void Translate(const Vector3 &u);
    void Scale(const Vector3 &s);
    void RotateX(double rad);
    void RotateY(double rad);
    void RotateZ(double rad);

    Vector3 TransformVector(const Vector3 &u) const { return m * u; }
    Point3 TransformNormal(const Vector3 &u) const { return mInvT * u; }
    Point3 TransformPoint(const Point3 &p) const { return m * p + v; }
    Ray TransformRay(const Ray &r) const { return Ray(m * r.origin + v, m * r.direction); }

    static Transform Inverse(const Transform &t);
    static Transform FromNormal(const Vector3 &n);
    static Transform LookAt(const Point3 &origin, const Point3 &target, const Vector3 &world_up);

private:
    Matrix3 m;
    Matrix3 mInvT;
    Vector3 v;
};

#endif