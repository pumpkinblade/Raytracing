#include "Bvh.h"
#include <algorithm>

using Parition = std::pair<std::vector<std::shared_ptr<Hittable>>, std::vector<std::shared_ptr<Hittable>>>;

Parition NaiveParition(std::vector<std::shared_ptr<Hittable>> objs)
{
    Bounds3 bounds;
    for (const auto &o : objs)
        bounds = Bounds3::Union(bounds, o->GetBounds());

    Vector3 diag = bounds.Max() - bounds.Min();
    int axis;
    if (diag.X() > diag.Y())
    {
        if (diag.X() > diag.Z())
            axis = 0;
        else
            axis = 2;
    }
    else
    {
        if (diag.Y() > diag.Z())
            axis = 1;
        else
            axis = 2;
    }
    auto comp = [axis](const std::shared_ptr<Hittable> &b1, const std::shared_ptr<Hittable> &b2)
    {
        double c1 = b1->GetBounds().Min()(axis) + b1->GetBounds().Max()(axis);
        double c2 = b2->GetBounds().Min()(axis) + b2->GetBounds().Max()(axis);
        return c1 < c2;
    };
    std::sort(objs.begin(), objs.end(), comp);
    auto mid = objs.size() / 2;
    auto left = std::vector(objs.begin(), objs.begin() + mid);
    auto right = std::vector(objs.begin() + mid, objs.end());
    return std::make_pair(left, right);
}

Parition SAHParition(std::vector<std::shared_ptr<Hittable>> objs)
{
    Bounds3 bounds;
    for (const auto &o : objs)
        bounds = Bounds3::Union(bounds, o->GetBounds());

    constexpr int B = 10;
    // 先计算按哪一条轴、哪一个桶划分最好
    double bestCost = FT_MAX;
    double bestAxis = 0;
    double bestBucketIndex = 0;
    Vector3 diag = bounds.Diagonal();
    for (int axis = 0; axis < 3; axis++)
    {
        // 初始化桶子
        std::vector<Bounds3> bucketBounds(B);
        std::vector<int> bucketPrims(B);
        for (auto &prim : bucketPrims)
            prim = 0;

        // 若一个轴长度太小就跳过
        double axisLen = diag(axis);
        if (axisLen < EPSILON)
            continue;

        // 将物体放入桶子
        double invAxisLen = 1.0 / axisLen;
        for (const auto &obj : objs)
        {
            double centeriod = obj->GetBounds().Centeriod()(axis);
            int bIndex = static_cast<int>((centeriod - bounds.Min()(axis)) * invAxisLen * B);
            bIndex = Clamp(bIndex, 0, B - 1);
            bucketBounds[bIndex] = Bounds3::Union(obj->GetBounds(), bucketBounds[bIndex]);
            bucketPrims[bIndex]++;
        }

        // 寻找最好的划分方法
        for (int p = 0; p < B - 1; p++)
        {
            // bucket[0..p] | bucket[p+1..B-1]
            Bounds3 b1, b2;
            int n1 = 0, n2 = 0;
            for (int i = 0; i <= p; i++)
            {
                b1 = Bounds3::Union(b1, bucketBounds[i]);
                n1 += bucketPrims[i];
            }
            for (int i = p + 1; i <= B - 1; i++)
            {
                b2 = Bounds3::Union(b2, bucketBounds[i]);
                n2 += bucketPrims[i];
            }

            double cost = b1.Area() * n1 + b2.Area() * n2;
            if (cost < bestCost)
            {
                bestCost = cost;
                bestAxis = axis;
                bestBucketIndex = p;
            }
        }
    }

    if (bestCost == FT_MAX)
    {
        return NaiveParition(objs);
    }
    else
    {
        // 划分左右
        std::vector<std::shared_ptr<Hittable>> left, right;
        double invAxisLen = 1.0 / diag(bestAxis);
        for (const auto &obj : objs)
        {
            double centeriod = obj->GetBounds().Centeriod()(bestAxis);
            int bIndex = static_cast<int>((centeriod - bounds.Min()(bestAxis)) * invAxisLen * B);
            bIndex = Clamp(bIndex, 0, B - 1);
            if (bIndex <= bestBucketIndex)
                left.push_back(obj);
            else
                right.push_back(obj);
        }
        return std::make_pair(left, right);
    }
}

BvhNode::BvhNode(const std::vector<std::shared_ptr<Hittable>> &objs, BvhParitionMethod method)
{
    if (objs.size() == 1)
    {
        object = objs[0];
        left = right = nullptr;
        bounds = objs[0]->GetBounds();
    }
    else if (objs.size() == 2)
    {
        left = std::make_shared<BvhNode>(std::vector{objs[0]});
        right = std::make_shared<BvhNode>(std::vector{objs[1]});
        object = nullptr;
        bounds = Bounds3::Union(objs[0]->GetBounds(), objs[1]->GetBounds());
    }
    else
    {
        switch (method)
        {
        case BvhParitionMethod::SAH:
        {
            auto parition = SAHParition(objs);
            left = std::make_shared<BvhNode>(parition.first, method);
            right = std::make_shared<BvhNode>(parition.second, method);
            object = nullptr;
            bounds = Bounds3::Union(left->bounds, right->bounds);
        }
        break;
        case BvhParitionMethod::NAIVE:
        default:
        {
            auto parition = NaiveParition(objs);
            left = std::make_shared<BvhNode>(parition.first, method);
            right = std::make_shared<BvhNode>(parition.second, method);
            object = nullptr;
            bounds = Bounds3::Union(left->bounds, right->bounds);
        }
        break;
        }
    }
}

bool BvhNode::Hit(const Ray &ray, const Vector3 &dirInv, const Vector3 &dirIsNeg, HitRecord &rec) const
{
    if (!bounds.HitP(ray, dirInv, dirIsNeg))
        return false;
    if (left != nullptr && right != nullptr)
    {
        HitRecord rec_left;
        bool hit_left = left->Hit(ray, dirInv, dirIsNeg, rec_left);
        HitRecord rec_right;
        bool hit_right = right->Hit(ray, dirInv, dirIsNeg, rec_right);
        if (hit_left && hit_right)
        {
            if (rec_left.t < rec_right.t)
                rec = rec_left;
            else
                rec = rec_right;
            return true;
        }
        else if (hit_left)
        {
            rec = rec_left;
            return true;
        }
        else if (hit_right)
        {
            rec = rec_right;
            return true;
        }
        return false;
    }
    else
    {
        if (object != nullptr)
            return object->Hit(ray, rec);
        else
            return false;
    }
}

Bvh::Bvh(const std::vector<std::shared_ptr<Hittable>> &objs, BvhParitionMethod method)
{
    objects = objs;
    root = std::make_shared<BvhNode>(objects, method);
}

bool Bvh::Hit(const Ray &ray, HitRecord &rec) const
{
    if (root == nullptr)
        return false;
    Vector3 dirInv = Vector3(1.0 / ray.direction.X(), 1.0 / ray.direction.Y(), 1.0 / ray.direction.Z());
    Vector3 dirIsNeg = Vector3(ray.direction.X() < 0, ray.direction.Y() < 0, ray.direction.Z() < 0);
    return root->Hit(ray, dirInv, dirIsNeg, rec);
}

Vector3 Bvh::Sample(const Point3 &o) const
{
    int idx = RandomInt(0, objects.size());
    return objects[idx]->Sample(o);
}

double Bvh::Pdf(const Point3 &o, const Vector3 &v) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->Pdf(o, v);

    return sum;
}

Bounds3 Bvh::GetBounds() const
{
    return root->bounds;
}
