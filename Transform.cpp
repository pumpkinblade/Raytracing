#include "Transform.h"
#include <cstring>

Transform::Transform()
    : m(1.0), v(0.0), mInvT(1.0)
{
}

Transform::Transform(const Matrix3 &mm, const Vector3 &vv)
    : m(mm), v(vv)
{
    mInvT = Matrix3::Inverse(m);
}

Transform::Transform(const Point3 &o, const Vector3 &uu, const Vector3 &vv, const Vector3 &ww)
{
    for (int i = 0; i < 3; i++)
    {
        m(i, 0) = uu(i);
        m(i, 1) = vv(i);
        m(i, 2) = ww(i);
    }
    v = o;
    mInvT = Matrix3::Inverse(Matrix3::Transpose(m));
}

void Transform::Translate(const Vector3 &u)
{
    v += u;
}

void Transform::Scale(const Vector3 &s)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m(i, j) *= s(i);
            mInvT(i, j) /= s(i);
        }
        v(i) *= s(i);
    }
}

void Transform::RotateX(double rad)
{
    double cosine = std::cos(rad);
    double sine = std::sin(rad);
    double t1, t2;
    for (int j = 0; j < 3; j++)
    {
        t1 = m(1, j);
        t2 = m(2, j);
        m(1, j) = cosine * t1 - sine * t2;
        m(2, j) = sine * t1 + cosine * t2;

        t1 = mInvT(1, j);
        t2 = mInvT(2, j);
        mInvT(1, j) = cosine * t1 - sine * t2;
        mInvT(2, j) = sine * t1 + cosine * t2;
    }
    t1 = v(1);
    t2 = v(2);
    v(1) = cosine * t1 - sine * t2;
    v(2) = sine * t1 + cosine * t2;
}

void Transform::RotateY(double rad)
{
    double cosine = std::cos(rad);
    double sine = std::sin(rad);
    double t1, t2;
    for (int j = 0; j < 3; j++)
    {
        t1 = m(2, j);
        t2 = m(0, j);
        m(2, j) = cosine * t1 - sine * t2;
        m(0, j) = sine * t1 + cosine * t2;

        t1 = mInvT(2, j);
        t2 = mInvT(0, j);
        mInvT(2, j) = cosine * t1 - sine * t2;
        mInvT(0, j) = sine * t1 + cosine * t2;
    }
    t1 = v(2);
    t2 = v(0);
    v(2) = cosine * t1 - sine * t2;
    v(0) = sine * t1 + cosine * t2;
}

void Transform::RotateZ(double rad)
{
    double cosine = std::cos(rad);
    double sine = std::sin(rad);
    double t1, t2;
    for (int j = 0; j < 3; j++)
    {
        t1 = m(0, j);
        t2 = m(1, j);
        m(0, j) = cosine * t1 - sine * t2;
        m(1, j) = sine * t1 + cosine * t2;

        t1 = mInvT(0, j);
        t2 = mInvT(1, j);
        mInvT(0, j) = cosine * t1 - sine * t2;
        mInvT(1, j) = sine * t1 + cosine * t2;
    }
    t1 = v(0);
    t2 = v(1);
    v(0) = cosine * t1 - sine * t2;
    v(1) = sine * t1 + cosine * t2;
}

Transform Transform::Inverse(const Transform &t)
{
    Matrix3 minv = Matrix3::Inverse(t.m);
    Vector3 vinv = minv * -t.v;
    return Transform(minv, vinv);
}

Transform Transform::FromNormal(const Vector3 &n)
{
    auto w = Normalize(n);
    auto a = std::abs(w.X()) > 0.9 ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    auto v = Normalize(Cross(w, a));
    auto u = Cross(v, w);

    return Transform(Vector3(0.0, 0.0, 0.0), u, v, w);
}

Transform Transform::LookAt(const Point3 &origin, const Point3 &target, const Vector3 &world_up)
{
    auto w = Normalize(target - origin);
    auto u = Normalize(Cross(world_up, w));
    auto v = Cross(w, u);
    return Transform(origin, u, v, w);
}