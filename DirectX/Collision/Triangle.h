#pragma once

#include "../Math/Math.h"

struct Triangle {
    Vector3 p0;
    Vector3 p1;
    Vector3 p2;

    Triangle();
    Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    Vector3 normal() const;
};
