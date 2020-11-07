#include "Vector2.h"
#include "Math.h"
#include "Matrix3.h"

Vector2::Vector2() :
    x(0.f),
    y(0.f) {
}

Vector2::Vector2(float inX, float inY) :
    x(inX),
    y(inY) {
}

void Vector2::set(float inX, float inY) {
    x = inX;
    y = inY;
}

Vector2 Vector2::operator-() {
    return Vector2(-x, -y);
}

Vector2 operator-(const Vector2& vec) {
    return Vector2(-vec.x, -vec.y);
}

Vector2 operator+(const Vector2& a, const Vector2& b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator-(const Vector2& a, const Vector2& b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator*(const Vector2& a, const Vector2& b) {
    return Vector2(a.x * b.x, a.y * b.y);
}

Vector2 operator*(const Vector2& vec, float scalar) {
    return Vector2(vec.x * scalar, vec.y * scalar);
}

Vector2 operator*(float scalar, const Vector2& vec) {
    return Vector2(vec.x * scalar, vec.y * scalar);
}

Vector2 operator/(const Vector2& vec, float scalar) {
    if (scalar == 0) {
        return vec;
    }
    return Vector2(vec.x / scalar, vec.y / scalar);
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& right) {
    x += right.x;
    y += right.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& right) {
    x -= right.x;
    y -= right.y;
    return *this;
}

bool Vector2::equal(const Vector2& right) const {
    return (Math::equal(x, right.x) && Math::equal(y, right.y));
}

float Vector2::lengthSq() const {
    return (x * x + y * y);
}

float Vector2::length() const {
    return (Math::sqrt(lengthSq()));
}

float Vector2::distance(const Vector2& a, const Vector2& b) {
    auto dist = a - b;
    return dist.length();
}

void Vector2::clamp(const Vector2& min, const Vector2& max) {
    x = Math::clamp<float>(x, min.x, max.x);
    y = Math::clamp<float>(y, min.y, max.y);
}

Vector2 Vector2::clamp(const Vector2& value, const Vector2& min, const Vector2& max) {
    Vector2 temp = value;
    temp.x = Math::clamp<float>(temp.x, min.x, max.x);
    temp.y = Math::clamp<float>(temp.y, min.y, max.y);
    return temp;
}

void Vector2::normalize() {
    float len = length();
    if (len == 0) {
        return;
    }
    x /= len;
    y /= len;
}

Vector2 Vector2::normalize(const Vector2& vec) {
    Vector2 temp = vec;
    temp.normalize();
    return temp;
}

float Vector2::dot(const Vector2& a, const Vector2& b) {
    return (a.x * b.x + a.y * b.y);
}

float Vector2::cross(const Vector2& a, const Vector2& b) {
    return (a.x * b.y - a.y * b.x);
}

Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float f) {
    return Vector2(a + f * (b - a));
}

Vector2 Vector2::reflect(const Vector2& v, const Vector2& n) {
    return v - 2.f * Vector2::dot(v, n) * n;
}

Vector2 Vector2::transform(const Vector2& vec, const Matrix3& mat, float w) {
    Vector2 retVal;
    retVal.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + w * mat.m[2][0];
    retVal.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + w * mat.m[2][1];
    //ignore w since we aren't returning a new value for it...
    return retVal;
}

const Vector2 Vector2::zero(0.f, 0.f);
const Vector2 Vector2::right(1.f, 0.f);
const Vector2 Vector2::up(0.f, -1.f);
const Vector2 Vector2::left(-1.f, 0.f);
const Vector2 Vector2::down(0.f, 1.f);
const Vector2 Vector2::one(1.f, 1.f);
