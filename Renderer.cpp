#include "Renderer.h"
#include <iostream>
#include <fstream>

Color CastRay(const Ray &ray, const std::shared_ptr<Hittable> &scene, const std::shared_ptr<Hittable> &light, const Color &background, int depth)
{
    HitRecord rec;
    if (depth <= 0)
        return background;

    if (!scene->Hit(ray, rec))
        return background;

    Vector3 wi = Normalize(-ray.direction);
    Vector3 wo;
    Vector3 emitted = rec.material->Emit(rec.position, rec.normal, wi);
    if (!rec.material->Scatter(rec.position, rec.normal, wi, wo))
        return emitted;

    if (RandomFloat() < 0.5)
        wo = light->Sample(rec.position);
    double pdf = 0.5 * light->Pdf(rec.position, wo) + 0.5 * rec.material->Pdf(rec.position, rec.normal, wi, wo);

    return emitted + rec.material->Brdf(rec.position, rec.normal, wi, wo) * CastRay(Ray(rec.position + wo * DELTA, wo), scene, light, background, depth - 1) * std::max(Dot(rec.normal, wo), 0.0) / std::max(pdf, EPSILON);
}

void Renderer::SetScene(const std::shared_ptr<Hittable> &sc)
{
    scene = sc;
}

void Renderer::SetLight(const std::shared_ptr<Hittable> &li)
{
    light = li;
}

void Renderer::SetCamera(const Camera &cam)
{
    camera = cam;
}

void Renderer::SetSample(int s)
{
    spp = s;
    oneDivSpp = 1.0 / s;
}

void Renderer::SetFramebuffer(int w, int h)
{
    width = w;
    height = h;
    framebuffer.resize(width * height);
}

void Renderer::SetBackground(const Color &bg)
{
    background = bg;
}

void Renderer::SetMaxDepth(int depth)
{
    maxDepth = depth;
}

void Renderer::Render()
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Ray ray = camera.GetRay(i / static_cast<double>(width), 1.0 - j / static_cast<double>(height));
            int idx = j * width + i;
            for (int s = 0; s < spp; s++)
            {
                framebuffer[idx] += CastRay(ray, scene, light, background, maxDepth) * oneDivSpp;
            }
        }
        std::cout << "\rRemaining line: " << (height - j - 1) << ' ' << std::flush;
    }
}

void Renderer::ToBmp(const std::string &file, bool flipY)
{
    // save framebuffer to file (BMP)
    int rowSize = 4 * ((width * 3 + 3) / 4);
    int rowPad = rowSize - width * 3;
    int dataSize = rowSize * height;
    int bmpSize = dataSize + 54;
    std::ofstream of(file, std::ios::binary | std::ios::out);
    of.write("BM", 2);
    of.write((char *)&bmpSize, sizeof(int));
    int tmp;
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));
    tmp = 54;
    of.write((char *)&tmp, sizeof(int));
    tmp = 40;
    of.write((char *)&tmp, sizeof(int));
    of.write((char *)&width, sizeof(int));
    of.write((char *)&height, sizeof(int));
    tmp = 0x00180001;
    of.write((char *)&tmp, sizeof(int));
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));
    of.write((char *)&dataSize, sizeof(int));
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));
    tmp = 0;
    of.write((char *)&tmp, sizeof(int));

    for (int j = 0; j < height; j++)
    {
        int k = flipY ? height - j - 1 : j;
        for (int i = 0; i < width; i++)
        {
            unsigned char bgr[3];
            double r = std::sqrt(framebuffer[k * width + i].X());
            double g = std::sqrt(framebuffer[k * width + i].Y());
            double b = std::sqrt(framebuffer[k * width + i].Z());
            bgr[0] = static_cast<unsigned char>(Clamp(b, 0.0, 1.0) * 255);
            bgr[1] = static_cast<unsigned char>(Clamp(g, 0.0, 1.0) * 255);
            bgr[2] = static_cast<unsigned char>(Clamp(r, 0.0, 1.0) * 255);
            of.write((char *)bgr, 3);
        }
        tmp = 0;
        of.write((char *)&tmp, rowPad);
    }
}