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

void PivotFunc::pivotToString(const Pivot& src, std::string& dst) {
    if (src == Pivot::LEFT_TOP) {
        dst = "LeftTop";
    } else if (src == Pivot::CENTER_TOP) {
        dst = "CenterTop";
    } else if (src == Pivot::RIGHT_TOP) {
        dst = "RightTop";
    } else if (src == Pivot::CENTER_LEFT) {
        dst = "CenterLeft";
    } else if (src == Pivot::CENTER) {
        dst = "Center";
    } else if (src == Pivot::CENTER_RIGHT) {
        dst = "CenterRight";
    } else if (src == Pivot::LEFT_BOTTOM) {
        dst = "LeftBottom";
    } else if (src == Pivot::CENTER_LEFT) {
        dst = "CenterBottom";
    } else if (src == Pivot::RIGHT_BOTTOM) {
        dst = "RightBottom";
    } else if (src == Pivot::NONE) {
        dst = "LeftTop";
    }
}
