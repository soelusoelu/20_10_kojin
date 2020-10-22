#include "Matrix3.h"
#include "Math.h"
#include "Vector2.h"
#include <memory>

Matrix3::Matrix3() {
    *this = Matrix3::identity;
}

Matrix3::Matrix3(float inMat[3][3]) {
    memcpy(m, inMat, 9 * sizeof(float));
}

Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
    Matrix3 retVal;
    // row 0
    retVal.m[0][0] =
        left.m[0][0] * right.m[0][0] +
        left.m[0][1] * right.m[1][0] +
        left.m[0][2] * right.m[2][0];

    retVal.m[0][1] =
        left.m[0][0] * right.m[0][1] +
        left.m[0][1] * right.m[1][1] +
        left.m[0][2] * right.m[2][1];

    retVal.m[0][2] =
        left.m[0][0] * right.m[0][2] +
        left.m[0][1] * right.m[1][2] +
        left.m[0][2] * right.m[2][2];

    // row 1
    retVal.m[1][0] =
        left.m[1][0] * right.m[0][0] +
        left.m[1][1] * right.m[1][0] +
        left.m[1][2] * right.m[2][0];

    retVal.m[1][1] =
        left.m[1][0] * right.m[0][1] +
        left.m[1][1] * right.m[1][1] +
        left.m[1][2] * right.m[2][1];

    retVal.m[1][2] =
        left.m[1][0] * right.m[0][2] +
        left.m[1][1] * right.m[1][2] +
        left.m[1][2] * right.m[2][2];

    // row 2
    retVal.m[2][0] =
        left.m[2][0] * right.m[0][0] +
        left.m[2][1] * right.m[1][0] +
        left.m[2][2] * right.m[2][0];

    retVal.m[2][1] =
        left.m[2][0] * right.m[0][1] +
        left.m[2][1] * right.m[1][1] +
        left.m[2][2] * right.m[2][1];

    retVal.m[2][2] =
        left.m[2][0] * right.m[0][2] +
        left.m[2][1] * right.m[1][2] +
        left.m[2][2] * right.m[2][2];

    return retVal;
}

Matrix3& Matrix3::operator*=(const Matrix3& right) {
    *this = *this * right;
    return *this;
}

Matrix3 Matrix3::createScale(float xScale, float yScale) {
    float temp[3][3] = {
        { xScale, 0.f, 0.f },
        { 0.f, yScale, 0.f },
        { 0.f, 0.f, 1.f },
    };
    return Matrix3(temp);
}

Matrix3 Matrix3::createScale(const Vector2& scaleVector) {
    return createScale(scaleVector.x, scaleVector.y);
}

Matrix3 Matrix3::createScale(float scale) {
    return createScale(scale, scale);
}

Matrix3 Matrix3::createRotation(float theta) {
    float temp[3][3] = {
        { Math::cos(theta), Math::sin(theta), 0.f },
        { -Math::sin(theta), Math::cos(theta), 0.f },
        { 0.f, 0.f, 1.f },
    };
    return Matrix3(temp);
}

Matrix3 Matrix3::createTranslation(const Vector2& trans) {
    float temp[3][3] = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { trans.x, trans.y, 1.f },
    };
    return Matrix3(temp);
}

float m3Ident[3][3] = {
    { 1.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f },
    { 0.f, 0.f, 1.f }
};
const Matrix3 Matrix3::identity(m3Ident);
