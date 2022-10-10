#ifndef CAMERA_H
#define CAMERA_H

#include "rtbase.h"
#include "Transform.h"
#include <memory>
#include <string>

class Camera
{
public:
    Camera() = default;

    Camera(const Point3 &position, const Point3 &look_at, const Vector3 &world_up, double fovY, double aspect_ratio)
    {
        transform = Transform::LookAt(position, look_at, world_up);
        vert = std::tan(0.5 * fovY);
        hori = aspect_ratio * vert;
    }

    Ray GetRay(double u, double v) const
    {
        Ray ray(Point3(0.0), Vector3((2 * u - 1) * hori, (2 * v - 1) * vert, 1.0));
        return transform.TransformRay(ray);
    }

    // void SetFlipY(bool on) { flipY = on; }
    // void SetSamples(int s) { oneDivSamples = 1.0 / s; }
    // void WritePixel(int x, int y, Color pixel);
    // void ToBmp(const std::string& file) const;
private:
    double hori, vert;
    Transform transform;

    // int width, height;
    // std::unique_ptr<unsigned char[]> image;
    // bool flipY;
    // double oneDivSamples;
};

#endif