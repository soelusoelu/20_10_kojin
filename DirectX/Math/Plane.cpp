#include "Plane.h"
#include "Vector3.h"

Plane::Plane(float inA, float inB, float inC, float inD) :
    a(inA),
    b(inB),
    c(inC),
    d(inD) {
}

Plane::Plane(const Vector3& normal, float d) :
    a(normal.x),
    b(normal.y),
    c(normal.z),
    d(d) {
}

Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    auto n = Vector3::cross(v1, v2);
    a = n.x;
    b = n.y;
    c = n.z;
    d = Vector3::dot(n, p1);
}

Plane::Plane(const Vector3& normal, const Vector3& p) {
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = Vector3::dot(p, normal);
}

Vector3 Plane::normal() const {
    return Vector3(a, b, c);
}

float Plane::dotNormal(const Vector3& vector) const {
    return Vector3::dot(normal(), vector);
}

void Plane::normalize() {
    auto n = normal();
    d = d / n.length();
    n.normalize();
    a = n.x;
    b = n.y;
    c = n.z;
}
