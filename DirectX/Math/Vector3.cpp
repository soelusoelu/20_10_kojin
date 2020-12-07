#include "Vector3.h"
#include "Math.h"
#include "Matrix4.h"
#include "Vector2.h"

Vector3::Vector3() :
    x(0.f),
    y(0.f),
    z(0.f) {
}

Vector3::Vector3(float inX, float inY, float inZ) :
    x(inX),
    y(inY),
    z(inZ) {
}

Vector3::Vector3(const Vector2& vec2, float inZ) :
    x(vec2.x),
    y(vec2.y),
    z(inZ) {
}

void Vector3::set(float inX, float inY, float inZ) {
    x = inX;
    y = inY;
    z = inZ;
}

Vector3& Vector3::operator=(const Vector3& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator*(const Vector3& left, const Vector3& right) {
    return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3 operator*(const Vector3& vec, float scalar) {
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector3 operator*(float scalar, const Vector3& vec) {
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector3 operator/(const Vector3& vec, float scalar) {
    if (scalar == 0) {
        return vec;
    }
    return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator*=(const Vector3& right) {
    x *= right.x;
    y *= right.y;
    z *= right.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& right) {
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& right) {
    x -= right.x;
    y -= right.y;
    z -= right.z;
    return *this;
}

bool Vector3::equal(const Vector3& right) const {
    return (Math::equal(x, right.x) && Math::equal(y, right.y) && Math::equal(z, right.z));
}

bool Vector3::equal(const Vector3& left, const Vector3& right) {
    return (Math::equal(left.x, right.x) && Math::equal(left.y, right.y) && Math::equal(left.z, right.z));
}

float Vector3::lengthSq() const {
    return (x * x + y * y + z * z);
}

float Vector3::length() const {
    return (Math::sqrt(lengthSq()));
}

float Vector3::distance(const Vector3& a, const Vector3& b) {
    auto dist = a - b;
    return dist.length();
}

void Vector3::clamp(const Vector3& min, const Vector3& max) {
    x = Math::clamp<float>(x, min.x, max.x);
    y = Math::clamp<float>(y, min.y, max.y);
    z = Math::clamp<float>(z, min.z, max.z);
}

Vector3 Vector3::clamp(const Vector3& value, const Vector3& min, const Vector3& max) {
    Vector3 temp = value;
    temp.x = Math::clamp<float>(temp.x, min.x, max.x);
    temp.y = Math::clamp<float>(temp.y, min.y, max.y);
    temp.z = Math::clamp<float>(temp.z, min.z, max.z);
    return temp;
}

void Vector3::normalize() {
    float len = length();
    if (len == 0) {
        return;
    }
    x /= len;
    y /= len;
    z /= len;
}

Vector3 Vector3::normalize(const Vector3& vec) {
    Vector3 temp = vec;
    temp.normalize();
    return temp;
}

float Vector3::dot(const Vector3& a, const Vector3& b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float f) {
    return Vector3(a + f * (b - a));
}

Vector3 Vector3::reflect(const Vector3& v, const Vector3& n) {
    return v - 2.0f * Vector3::dot(v, n) * n;
}

Vector3 Vector3::transform(const Vector3& vec, const Matrix4& mat, float w) {
    Vector3 retVal;
    retVal.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + w * mat.m[3][0];
    retVal.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + w * mat.m[3][1];
    retVal.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + w * mat.m[3][2];
    //ignore w since we aren't returning a new value for it...
    return retVal;
}

Vector3 Vector3::transformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w) {
    Vector3 retVal;
    retVal.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + w * mat.m[3][0];
    retVal.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + w * mat.m[3][1];
    retVal.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + w * mat.m[3][2];
    float transformedW = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + w * mat.m[3][3];
    if (!Math::nearZero(Math::abs(transformedW))) {
        transformedW = 1.f / transformedW;
        retVal *= transformedW;
    }
    return retVal;
}

Vector3 Vector3::transform(const Vector3& v, const Quaternion& q) {
    // v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
    Vector3 qv(q.x, q.y, q.z);
    Vector3 retVal = v;
    retVal += 2.f * Vector3::cross(qv, Vector3::cross(qv, v) + q.w * v);
    return retVal;
}

const Vector3 Vector3::zero(0.f, 0.f, 0.f);
const Vector3 Vector3::right(1.f, 0.f, 0.f);
const Vector3 Vector3::up(0.f, 1.f, 0.f);
const Vector3 Vector3::forward(0.f, 0.f, 1.f);
const Vector3 Vector3::left(-1.f, 0.f, 0.f);
const Vector3 Vector3::down(0.f, -1.f, 0.f);
const Vector3 Vector3::back(0.f, 0.f, -1.f);
const Vector3 Vector3::one(1.f, 1.f, 1.f);
const Vector3 Vector3::negOne(-1.f, -1.f, -1.f);
const Vector3 Vector3::infinity(Math::infinity, Math::infinity, Math::infinity);
const Vector3 Vector3::negInfinity(Math::negInfinity, Math::negInfinity, Math::negInfinity);
