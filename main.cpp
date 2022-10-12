#include "Renderer.h"
#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"
#include "HittableList.h"
#include "TransformedObject.h"
#include "Bvh.h"
#include <iostream>

int main(void)
{
    auto camPos = Point3(278, 278, -800);
    auto camTarget = Point3(278, 278, 0);
    auto camUp = Vector3(0.0, 1.0, 0.0);
    auto camFovY = DegreeToRadian(40);
    auto camAspectRatio = 1.0;
    auto background = Color(0, 0, 0);
    int imWidth = 600, imHeight = 600;
    int maxDepth = 50;
    int spp = 100;

    Camera cam(camPos, camTarget, camUp, camFovY, camAspectRatio);

    auto scene = std::make_shared<HittableList>();
    auto light = std::make_shared<HittableList>();

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto litMat = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    // 左面
    Point3 left[4] = {Point3(0, 0, 0), Point3(0, 555, 0), Point3(0, 555, 555), Point3(0, 0, 555)};
    scene->Add(std::make_shared<Triangle>(left[0], left[1], left[2], green));
    scene->Add(std::make_shared<Triangle>(left[0], left[2], left[3], green));
    // 右面
    Point3 right[4] = {Point3(555, 0, 0), Point3(555, 555, 0), Point3(555, 555, 555), Point3(555, 0, 555)};
    scene->Add(std::make_shared<Triangle>(right[0], right[2], right[1], red));
    scene->Add(std::make_shared<Triangle>(right[0], right[3], right[2], red));
    // 底面
    Point3 bottom[4] = {Point3(0, 0, 0), Point3(0, 0, 555), Point3(555, 0, 555), Point3(555, 0, 0)};
    scene->Add(std::make_shared<Triangle>(bottom[0], bottom[1], bottom[2], white));
    scene->Add(std::make_shared<Triangle>(bottom[0], bottom[2], bottom[3], white));
    // 顶面
    Point3 top[4] = {Point3(0, 555, 0), Point3(0, 555, 555), Point3(555, 555, 555), Point3(555, 555, 0)};
    scene->Add(std::make_shared<Triangle>(top[0], top[2], top[1], white));
    scene->Add(std::make_shared<Triangle>(top[0], top[3], top[2], white));
    // 背面
    Point3 back[4] = {Point3(0, 0, 555), Point3(0, 555, 555), Point3(555, 555, 555), Point3(555, 0, 555)};
    scene->Add(std::make_shared<Triangle>(back[0], back[1], back[2], white));
    scene->Add(std::make_shared<Triangle>(back[0], back[2], back[3], white));
    // 灯
    Point3 lightPlane[4] = {Point3(213, 554, 227), Point3(213, 554, 332), Point3(343, 554, 332), Point3(343, 554, 227)};
    auto litTri1 = std::make_shared<Triangle>(lightPlane[0], lightPlane[2], lightPlane[1], litMat);
    auto litTri2 = std::make_shared<Triangle>(lightPlane[0], lightPlane[3], lightPlane[2], litMat);
    light->Add(litTri1);
    light->Add(litTri2);
    scene->Add(litTri1);
    scene->Add(litTri2);

    // 箱子
    auto box = std::make_shared<HittableList>();
    // 箱子底面
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(1, 0, 1), Point3(0, 0, 1), white));
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(1, 0, 0), Point3(1, 0, 1), white));
    // 箱子顶面
    box->Add(std::make_shared<Triangle>(Point3(0, 1, 0), Point3(0, 1, 1), Point3(1, 1, 1), white));
    box->Add(std::make_shared<Triangle>(Point3(0, 1, 0), Point3(1, 1, 1), Point3(1, 1, 0), white));
    // 箱子左面
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(0, 1, 1), Point3(0, 1, 0), white));
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(0, 0, 1), Point3(0, 1, 1), white));
    // 箱子右面
    box->Add(std::make_shared<Triangle>(Point3(1, 0, 0), Point3(1, 1, 0), Point3(1, 1, 1), white));
    box->Add(std::make_shared<Triangle>(Point3(1, 0, 0), Point3(1, 1, 1), Point3(1, 0, 1), white));
    // 箱子前面
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(0, 1, 0), Point3(1, 1, 0), white));
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(1, 1, 0), Point3(1, 0, 0), white));
    // 箱子后面
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 1), Point3(1, 1, 1), Point3(0, 1, 1), white));
    box->Add(std::make_shared<Triangle>(Point3(0, 0, 1), Point3(1, 0, 1), Point3(1, 1, 1), white));

    auto box_bvh = std::make_shared<Bvh>(box->Objects());

    Transform t1;
    t1.Scale(Vector3(165, 330, 165));
    t1.RotateY(DegreeToRadian(-18));
    t1.Translate(Vector3(125, 0, 255));
    scene->Add(std::make_shared<TransformedObject>(box_bvh, t1));

    Transform t2;
    t2.Scale(Vector3(165, 165, 165));
    t2.RotateY(DegreeToRadian(15));
    t2.Translate(Vector3(265, 0, 135));
    scene->Add(std::make_shared<TransformedObject>(box_bvh, t2));

    auto scene_bvh = std::make_shared<Bvh>(scene->Objects());

    Renderer renderer;
    renderer.SetScene(scene_bvh);
    renderer.SetLight(light);
    renderer.SetCamera(cam);
    renderer.SetFramebuffer(imWidth, imHeight);
    renderer.SetSample(spp);
    renderer.SetBackground(background);
    renderer.SetMaxDepth(maxDepth);

    renderer.Render();
    renderer.ToBmp("cornellbox.bmp", false);

    return 0;
}