#include "Random.h"

void Random::initialize() {
    std::random_device rd;
    mt.seed(rd());
}

float Random::randomNormal() {
    return randomRange(0.f, 1.f);
}

int Random::randomRange(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max - 1);
    return dist(mt);
}

float Random::randomRange(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

Vector2 Random::randomRange(const Vector2& min, const Vector2& max) {
    Vector2 v = Vector2(randomNormal(), randomNormal());
    return min + (max - min) * v;
}

Vector3 Random::randomRange(const Vector3& min, const Vector3& max) {
    Vector3 v = Vector3(randomNormal(), randomNormal(), randomNormal());
    return min + (max - min) * v;
}
