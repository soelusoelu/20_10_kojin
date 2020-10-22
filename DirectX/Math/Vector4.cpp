#include "Vector4.h"
#include "Vector3.h"

Vector4::Vector4() :
    x(0.f),
    y(0.f),
    z(0.f),
    w(0.f) {
}

Vector4::Vector4(float inX, float inY, float inZ, float inW) :
    x(inX),
    y(inY),
    z(inZ),
    w(inW) {
}

Vector4::Vector4(const Vector3& vec3, float inW) :
    x(vec3.x),
    y(vec3.y),
    z(vec3.z),
    w(inW) {
}

Vector4& Vector4::operator=(const Vector4& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
    return *this;
}
