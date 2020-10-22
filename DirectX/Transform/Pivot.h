#pragma once

#include <string>

enum class Pivot {
    LEFT_TOP,
    CENTER_TOP,
    RIGHT_TOP,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    LEFT_BOTTOM,
    CETNER_BOTTOM,
    RIGHT_BOTTOM,

    NONE
};

namespace PivotFunc {
    //文字列をPivotに変換
    void stringToPivot(const std::string& src, Pivot* dst);
};
