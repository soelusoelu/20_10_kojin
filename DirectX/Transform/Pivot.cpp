#include "Pivot.h"

void PivotFunc::stringToPivot(const std::string& src, Pivot* dst) {
    auto pivot = Pivot::NONE;

    if (src == "LeftTop") {
        pivot = Pivot::LEFT_TOP;
    } else if (src == "CenterTop") {
        pivot = Pivot::CENTER_TOP;
    } else if (src == "RightTop") {
        pivot = Pivot::RIGHT_TOP;
    } else if (src == "CenterLeft") {
        pivot = Pivot::CENTER_LEFT;
    } else if (src == "Center") {
        pivot = Pivot::CENTER;
    } else if (src == "CenterRight") {
        pivot = Pivot::CENTER_RIGHT;
    } else if (src == "LeftBottom") {
        pivot = Pivot::LEFT_BOTTOM;
    } else if (src == "CenterBottom") {
        pivot = Pivot::CETNER_BOTTOM;
    } else if (src == "RightBottom") {
        pivot = Pivot::RIGHT_BOTTOM;
    }

    if (pivot != Pivot::NONE) {
        *dst = pivot;
    }
}
