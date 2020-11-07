#pragma once

#include "../Math/Math.h"

struct Square {
    Vector2 min;
    Vector2 max;

    Square();
    Square(const Vector2& min, const Vector2& max);
    bool contains(const Vector2& point) const;
};
