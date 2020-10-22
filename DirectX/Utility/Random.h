#pragma once

#include "../Math/Math.h"
#include <random>

class Random {
public:
    static void initialize();
    //0.f <= value <= 1.f
    static float randomNormal();
    //min <= value < max
    static int randomRange(int min, int max);
    //min <= value <= max
    static float randomRange(float min, float max);
    static Vector2 randomRange(const Vector2& min, const Vector2& max);
    static Vector3 randomRange(const Vector3& min, const Vector3& max);

private:
    Random() = delete;
    ~Random() = delete;
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

private:
    static inline std::mt19937 mt;
};

