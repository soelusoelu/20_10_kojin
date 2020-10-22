#include "Easing.h"
#include "../Math/Math.h"

float Easing::easeInSin(float value) {
    return 1.f - Math::cos((value * 180.f) / 2.f);
}

float Easing::easeOutSin(float value) {
    return Math::sin((value * 180.f) / 2.f);
}

float Easing::easeInOutSin(float value) {
    return -(Math::cos(value * 180.f) - 1.f) / 2.f;
}

float Easing::easeInCubic(float value) {
    return value * value * value;
}

float Easing::easeOutCubic(float value) {
    return 1.f - powf(1.f - value, 3);
}

float Easing::easeInOutCubic(float value) {
    return value < 0.5f ? 4.f * value * value * value : 1.f - powf(-2.f * value + 2.f, 3) / 2.f;
}

float Easing::easeInBack(float value) {
    static constexpr float C1 = 1.70158f;
    static constexpr float C3 = C1 + 1.f;

    return C3 * value * value * value - C1 * value * value;
}

float Easing::easeOutBack(float value) {
    static constexpr float C1 = 1.70158f;
    static constexpr float C3 = C1 + 1.f;

    return 1.f + C3 * powf(value - 1.f, 3) + C1 * powf(value - 1.f, 2);
}

float Easing::easeInOutBack(float value) {
    static constexpr float C1 = 1.70158f;
    static constexpr float C2 = C1 * 1.525f;

    return value < 0.5f
        ? (powf(2.f * value, 2) * ((C2 + 1.f) * 2.f * value - C2)) / 2.f
        : (powf(2.f * value - 2.f, 2) * ((C2 + 1.f) * (value * 2.f - 2.f) + C2) + 2.f) / 2.f;
}
