#ifndef FTL_H
#define FTL_H

#include "Vector3.h"
#include "Ray.h"
#include <limits>
#include <cstdlib>

constexpr double PI = 3.1415926535897932385;
constexpr double EPSILON = std::numeric_limits<double>::epsilon();
constexpr double DELTA = 0.000001;
constexpr double INFTY = std::numeric_limits<double>::infinity();
constexpr double FT_MIN = std::numeric_limits<double>::lowest();
constexpr double FT_MAX = std::numeric_limits<double>::max();

constexpr double DegreeToRadian(double degree)
{
    return degree / 180.0 * PI;
}

template<typename T>
constexpr T Clamp(T v, T low, T up)
{
    return std::max(low, std::min(v, up));
}

// 生成[0, 1)中的随机浮点数
inline double RandomFloat()
{
    return std::rand() / (static_cast<double>(RAND_MAX) + 1);
}

// 生成[min,max)的随机整数
inline int RandomInt(int min, int max)
{
    return min + std::rand() % (max - min);
}

// 生成[min, max)中的随机浮点数
inline double RandomFloat(double min, double max)
{
    double f = RandomFloat();
    return (1.0 - f) * min + f * max;
}

inline Vector3 RandomInSphere()
{
    double r1 = RandomFloat();
    double r2 = RandomFloat();
    double cos_phi = std::cos(2 * PI * r1);
    double sin_phi = std::sin(2 * PI * r1);
    double cos_theta = 1 - 2 * r2;
    double sin_theta = 2 * std::sqrt(r2 * (1 - r2));
    return Vector3(cos_phi * sin_theta, sin_phi * sin_theta, cos_theta);
}

inline Vector3 RandomInHemiSphere()
{
    double r1 = RandomFloat();
    double r2 = RandomFloat();
    double cos_phi = std::cos(2 * PI * r1);
    double sin_phi = std::sin(2 * PI * r1);
    double cos_theta = 1 - r2;
    double sin_theta = std::sqrt(r2 * (2 - r2));
    return Vector3(cos_phi * sin_theta, sin_phi * sin_theta, cos_theta);
}

inline Vector3 RandomInHemiSphereCos()
{
    double r1 = RandomFloat();
    double r2 = RandomFloat();
    double cos_phi = std::cos(2 * PI * r1);
    double sin_phi = std::sin(2 * PI * r1);
    double cos_theta = std::sqrt(1 - r2);
    double sin_theta = std::sqrt(r2);
    return Vector3(cos_phi * sin_theta, sin_phi * sin_theta, cos_theta);
}

inline Vector3 RandomToSphere(double radius, double distance_squared)
{
    auto r1 = RandomFloat();
    auto r2 = RandomFloat();
    auto cos_theta = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);
    auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);
    auto cos_phi = std::cos(2 * PI * r1);
    auto sin_phi = std::sin(2 * PI * r1);
    return Vector3(cos_phi * sin_theta, sin_phi * sin_theta, cos_theta);
}

#endif