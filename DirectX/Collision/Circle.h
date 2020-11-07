#pragma once

#include "../Math/Math.h"

struct Circle {
    Vector2 center;
    float radius;

    Circle();
    Circle(const Vector2& center, float radius);
    bool contains(const Vector2& point) const;
};
