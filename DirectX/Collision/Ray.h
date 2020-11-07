#pragma once

#include "../Math/Math.h"

struct Ray {
    Vector3 start;
    Vector3 end;

    Ray();
    Ray(const Vector3& origin, const Vector3& direction, float maxDistance = FLT_MAX);
    //線分上の点を返す [0, t, 1]
    Vector3 pointOnSegment(float t) const;
    //最短距離の2乗
    float minDistanceSquare(const Vector3& point) const;
    //2本の線分から最短距離の2乗を取得
    static float minDistanceSquare(const Ray& s1, const Ray& s2);
};
