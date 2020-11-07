#pragma once

#include "../Math/Math.h"

struct AABB {
    Vector3 min;
    Vector3 max;

    AABB();
    AABB(const Vector3& min, const Vector3& max);
    void updateMinMax(const Vector3& point);
    void rotate(const Quaternion& q);
    bool contains(const Vector3& point) const;
    float minDistanceSquare(const Vector3& point) const;
};
