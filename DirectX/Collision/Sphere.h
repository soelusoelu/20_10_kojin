#pragma once

#include "../Math/Math.h"

struct Sphere {
    Vector3 center;
    float radius;

    Sphere();
    Sphere(const Vector3& center, float radius);
    bool contains(const Vector3& point) const;
};
