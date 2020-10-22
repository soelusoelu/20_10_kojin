#pragma once

#include <cmath>
#include <limits>
#include <cfloat>

namespace Math {
    constexpr float PI = 3.1415926535f;
    constexpr float TwoPI = PI * 2.f;
    constexpr float PIOver2 = PI / 2.f;
    constexpr float infinity = std::numeric_limits<float>::infinity();
    constexpr float negInfinity = -std::numeric_limits<float>::infinity();
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    constexpr float deg2Rad = PI / 180.f;
    constexpr float rad2Deg = 180.f / PI;

    inline float toRadians(float degrees) {
        return degrees * PI / 180.0f;
    }

    inline float toDegrees(float radians) {
        return radians * 180.0f / PI;
    }

    inline bool nearZero(float val) {
        return (fabs(val) <= epsilon);
    }

    inline bool equal(float a, float b) {
        return (fabs(a - b) <= epsilon);
    }

    template <typename T>
    T Max(const T& a, const T& b) {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b) {
        return (a < b ? a : b);
    }

    template <typename T>
    T clamp(const T& value, const T& min, const T& max) {
        return Min(max, Max(min, value));
    }

    inline float abs(float value) {
        return fabs(value);
    }

    inline float cos(float degree) {
        return cosf(degree * deg2Rad);
    }

    inline float sin(float degree) {
        return sinf(degree * deg2Rad);
    }

    inline float tan(float degree) {
        return tanf(degree * deg2Rad);
    }

    inline float asin(float value) {
        return asinf(value);
    }

    inline float acos(float value) {
        return acosf(value);
    }

    inline float atan2(float y, float x) {
        return atan2f(y, x);
    }

    inline float cot(float degree) {
        return 1.f / tan(degree);
    }

    inline float lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

    inline float sqrt(float value) {
        return sqrtf(value);
    }

    inline float fmod(float numer, float denom) {
        return fmod(numer, denom);
    }
}
