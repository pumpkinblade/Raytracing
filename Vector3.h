#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <cmath>

struct Vector3
{
    Vector3() : v{0.0, 0.0, 0.0} {}
    Vector3(double xx) : v{xx, xx, xx} {}
    Vector3(double xx, double yy, double zz) : v{xx, yy, zz} {}
    const double &X() const { return v[0]; }
    const double &Y() const { return v[1]; }
    const double &Z() const { return v[2]; }
    double &X() { return v[0]; }
    double &Y() { return v[1]; }
    double &Z() { return v[2]; }
    const double &operator()(int i) const { return v[i]; }
    double &operator()(int i) { return v[i]; }

    Vector3 &operator+=(const Vector3 &other)
    {
        v[0] += other.v[0];
        v[1] += other.v[1];
        v[2] += other.v[2];
        return *this;
    }

    Vector3 &operator-=(const Vector3 &other)
    {
        v[0] -= other.v[0];
        v[1] -= other.v[1];
        v[2] -= other.v[2];
        return *this;
    }

    Vector3 &operator*=(double t)
    {
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }

    Vector3 &operator/=(double t)
    {
        double inv = 1 / t;
        v[0] *= inv;
        v[1] *= inv;
        v[2] *= inv;
        return *this;
    }

    double v[3];
};

inline Vector3 operator-(const Vector3 &u)
{
    return Vector3(-u.X(), -u.Y(), -u.Z());
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

inline Vector3 operator*(double a, const Vector3 &u)
{
    return Vector3(a * u.X(), a * u.Y(), a * u.Z());
}

inline Vector3 operator*(const Vector3 &u, double a)
{
    return a * u;
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u(0) * v(0), u(1) * v(1), u(2) * v(2));
}

inline Vector3 operator/(const Vector3 &u, double a)
{
    return (1 / a) * u;
}

inline double Dot(const Vector3 &u, const Vector3 &v)
{
    return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}

inline Vector3 Cross(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.Y() * v.Z() - u.Z() * v.Y(),
                   u.Z() * v.X() - u.X() * v.Z(),
                   u.X() * v.Y() - u.Y() * v.X());
}

inline double Length(const Vector3 &u)
{
    return std::sqrt(u.X() * u.X() + u.Y() * u.Y() + u.Z() * u.Z());
}

inline double LengthSquared(const Vector3 &u)
{
    return u.X() * u.X() + u.Y() * u.Y() + u.Z() * u.Z();
}

inline Vector3 Normalize(const Vector3 &u)
{
    return u / Length(u);
}

using Point3 = Vector3;
using Color = Vector3;

#endif