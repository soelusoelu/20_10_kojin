#include "Square.h"

Square::Square() :
    min(Vector2::zero),
    max(Vector2::zero) {
}

Square::Square(const Vector2& min, const Vector2& max) :
    min(min),
    max(max) {
}

bool Square::contains(const Vector2& point) const {
    bool no = (
        (point.x < min.x) ||
        (point.x > max.x) ||
        (point.y < min.y) ||
        (point.y > max.y)
    );
    return !no;
}
