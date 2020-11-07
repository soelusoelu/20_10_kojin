#include "Sphere.h"

Sphere::Sphere() :
    center(Vector3::zero),
    radius(0.f) {
}

Sphere::Sphere(const Vector3& center, float radius) :
    center(center),
    radius(radius) {
}

bool Sphere::contains(const Vector3& point) const {
    //中心と点との距離の2乗を求める
    float distSq = (center - point).lengthSq();
    return distSq <= (radius * radius);
}
