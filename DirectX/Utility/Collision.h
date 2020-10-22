#pragma once

#include "../Math/Math.h"

struct Ray {
    Ray(const Vector3& origin, const Vector3& direction, float maxDistance = Math::infinity);
    //線分上の点を返す 0 <= t <= 1
    Vector3 pointOnSegment(float t) const;
    //最短距離の2乗
    float minDistanceSquare(const Vector3& point) const;
    //2本の線分から最短距離の2乗を取得
    static float minDistanceSquare(const Ray& s1, const Ray& s2);

    Vector3 start;
    Vector3 end;
};

struct Circle {
    Circle();
    Circle(const Vector2& center, float radius);
    void set(const Vector2& center, float radius);
    bool contains(const Vector2& point) const;

    Vector2 center;
    float radius;
};

struct Sphere {
    Sphere();
    Sphere(const Vector3& center, float radius);
    void set(const Vector3& center, float radius);
    bool contains(const Vector3& point) const;

    Vector3 center;
    float radius;
};

//oriented bounding box つまり回転する直方体
//AABBよりはるかに計算コストが高い
struct OBB {
    Vector3 mCenter;
    Quaternion mRotation;
    Vector3 mExtents;
};

//交差判定
bool intersect(const Circle& a, const Circle& b);

bool intersect(const Sphere& a, const Sphere& b);

bool intersect(const Ray& r, const Sphere& s, float* outT);

bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float* t);
