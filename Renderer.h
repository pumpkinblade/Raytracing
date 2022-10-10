#ifndef RENDERER_H
#define RENDERER_H

#include "Hittable.h"
#include "Camera.h"
#include <memory>
#include <vector>
#include <string>

class Renderer
{
public:
    Renderer() = default;

    void SetScene(const std::shared_ptr<Hittable>& sc);
    void SetLight(const std::shared_ptr<Hittable>& li);
    void SetCamera(const Camera& cam);
    void SetSample(int s);
    void SetFramebuffer(int w, int h);
    void SetBackground(const Color& bg);
    void SetMaxDepth(int depth);

    void Render();
    void ToBmp(const std::string& file, bool flipY);
private:
    std::shared_ptr<Hittable> scene;
    std::shared_ptr<Hittable> light;
    Camera camera;
    int width, height;
    int spp;
    double oneDivSpp;
    std::vector<Color> framebuffer;
    Color background;
    int maxDepth;
};

#endif