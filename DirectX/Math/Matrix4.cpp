#include "Matrix4.h"
#include "Math.h"
#include "Quaternion.h"
#include "Vector3.h"
#include <memory>

Matrix4::Matrix4() {
    *this = Matrix4::identity;
}

Matrix4::Matrix4(float inMat[4][4]) {
    memcpy(m, inMat, 16 * sizeof(float));
}

Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
    Matrix4 retVal;
    // row 0
    retVal.m[0][0] =
        a.m[0][0] * b.m[0][0] +
        a.m[0][1] * b.m[1][0] +
        a.m[0][2] * b.m[2][0] +
        a.m[0][3] * b.m[3][0];

    retVal.m[0][1] =
        a.m[0][0] * b.m[0][1] +
        a.m[0][1] * b.m[1][1] +
        a.m[0][2] * b.m[2][1] +
        a.m[0][3] * b.m[3][1];

    retVal.m[0][2] =
        a.m[0][0] * b.m[0][2] +
        a.m[0][1] * b.m[1][2] +
        a.m[0][2] * b.m[2][2] +
        a.m[0][3] * b.m[3][2];

    retVal.m[0][3] =
        a.m[0][0] * b.m[0][3] +
        a.m[0][1] * b.m[1][3] +
        a.m[0][2] * b.m[2][3] +
        a.m[0][3] * b.m[3][3];

    // row 1
    retVal.m[1][0] =
        a.m[1][0] * b.m[0][0] +
        a.m[1][1] * b.m[1][0] +
        a.m[1][2] * b.m[2][0] +
        a.m[1][3] * b.m[3][0];

    retVal.m[1][1] =
        a.m[1][0] * b.m[0][1] +
        a.m[1][1] * b.m[1][1] +
        a.m[1][2] * b.m[2][1] +
        a.m[1][3] * b.m[3][1];

    retVal.m[1][2] =
        a.m[1][0] * b.m[0][2] +
        a.m[1][1] * b.m[1][2] +
        a.m[1][2] * b.m[2][2] +
        a.m[1][3] * b.m[3][2];

    retVal.m[1][3] =
        a.m[1][0] * b.m[0][3] +
        a.m[1][1] * b.m[1][3] +
        a.m[1][2] * b.m[2][3] +
        a.m[1][3] * b.m[3][3];

    // row 2
    retVal.m[2][0] =
        a.m[2][0] * b.m[0][0] +
        a.m[2][1] * b.m[1][0] +
        a.m[2][2] * b.m[2][0] +
        a.m[2][3] * b.m[3][0];

    retVal.m[2][1] =
        a.m[2][0] * b.m[0][1] +
        a.m[2][1] * b.m[1][1] +
        a.m[2][2] * b.m[2][1] +
        a.m[2][3] * b.m[3][1];

    retVal.m[2][2] =
        a.m[2][0] * b.m[0][2] +
        a.m[2][1] * b.m[1][2] +
        a.m[2][2] * b.m[2][2] +
        a.m[2][3] * b.m[3][2];

    retVal.m[2][3] =
        a.m[2][0] * b.m[0][3] +
        a.m[2][1] * b.m[1][3] +
        a.m[2][2] * b.m[2][3] +
        a.m[2][3] * b.m[3][3];

    // row 3
    retVal.m[3][0] =
        a.m[3][0] * b.m[0][0] +
        a.m[3][1] * b.m[1][0] +
        a.m[3][2] * b.m[2][0] +
        a.m[3][3] * b.m[3][0];

    retVal.m[3][1] =
        a.m[3][0] * b.m[0][1] +
        a.m[3][1] * b.m[1][1] +
        a.m[3][2] * b.m[2][1] +
        a.m[3][3] * b.m[3][1];

    retVal.m[3][2] =
        a.m[3][0] * b.m[0][2] +
        a.m[3][1] * b.m[1][2] +
        a.m[3][2] * b.m[2][2] +
        a.m[3][3] * b.m[3][2];

    retVal.m[3][3] =
        a.m[3][0] * b.m[0][3] +
        a.m[3][1] * b.m[1][3] +
        a.m[3][2] * b.m[2][3] +
        a.m[3][3] * b.m[3][3];

    return retVal;
}

Matrix4& Matrix4::operator*=(const Matrix4& right) {
    *this = *this * right;
    return *this;
}

void Matrix4::transpose() {
    float src[16];

    // Transpose matrix
    // row 1 to col 1
    src[0] = m[0][0];
    src[4] = m[0][1];
    src[8] = m[0][2];
    src[12] = m[0][3];

    // row 2 to col 2
    src[1] = m[1][0];
    src[5] = m[1][1];
    src[9] = m[1][2];
    src[13] = m[1][3];

    // row 3 to col 3
    src[2] = m[2][0];
    src[6] = m[2][1];
    src[10] = m[2][2];
    src[14] = m[2][3];

    // row 4 to col 4
    src[3] = m[3][0];
    src[7] = m[3][1];
    src[11] = m[3][2];
    src[15] = m[3][3];

    // Set it back
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = src[i * 4 + j];
        }
    }
}

void Matrix4::inverse() {
    // Thanks slow math
    // This is a really janky way to unroll everything...
    float tmp[12];
    float src[16];
    float dst[16];
    float det;

    // Transpose matrix
    // row 1 to col 1
    src[0] = m[0][0];
    src[4] = m[0][1];
    src[8] = m[0][2];
    src[12] = m[0][3];

    // row 2 to col 2
    src[1] = m[1][0];
    src[5] = m[1][1];
    src[9] = m[1][2];
    src[13] = m[1][3];

    // row 3 to col 3
    src[2] = m[2][0];
    src[6] = m[2][1];
    src[10] = m[2][2];
    src[14] = m[2][3];

    // row 4 to col 4
    src[3] = m[3][0];
    src[7] = m[3][1];
    src[11] = m[3][2];
    src[15] = m[3][3];

    // Calculate cofactors
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];

    dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
    dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];

    dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

    // Calculate determinant
    det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

    // Inverse of matrix is divided by determinant
    det = 1 / det;
    for (int j = 0; j < 16; j++) {
        dst[j] *= det;
    }

    // Set it back
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = dst[i * 4 + j];
        }
    }
}

Matrix4 Matrix4::inverse(const Matrix4& right) {
    auto temp = right;
    temp.inverse();
    return temp;
}

Vector3 Matrix4::getTranslation() const {
    return Vector3(m[3][0], m[3][1], m[3][2]);
}

Vector3 Matrix4::getXAxis() const {
    return Vector3::normalize(Vector3(m[0][0], m[0][1], m[0][2]));
}

Vector3 Matrix4::getYAxis() const {
    return Vector3::normalize(Vector3(m[1][0], m[1][1], m[1][2]));
}

Vector3 Matrix4::getZAxis() const {
    return Vector3::normalize(Vector3(m[2][0], m[2][1], m[2][2]));
}

Vector3 Matrix4::getScale() const {
    Vector3 retVal;
    retVal.x = Vector3(m[0][0], m[0][1], m[0][2]).length();
    retVal.y = Vector3(m[1][0], m[1][1], m[1][2]).length();
    retVal.z = Vector3(m[2][0], m[2][1], m[2][2]).length();
    return retVal;
}

Matrix4 Matrix4::createScale(float xScale, float yScale, float zScale) {
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, zScale, 0.f },
        { 0.f, 0.f, 0.f, 1.f }
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createScale(const Vector3& scaleVector) {
    return createScale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Matrix4 Matrix4::createScale(float scale) {
    return createScale(scale, scale, scale);
}

Matrix4 Matrix4::createRotationX(float theta) {
    float temp[4][4] = {
        { 1.f, 0.f, 0.f , 0.f },
        { 0.f, Math::cos(theta), Math::sin(theta), 0.f },
        { 0.f, -Math::sin(theta), Math::cos(theta), 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createRotationY(float theta) {
    float temp[4][4] = {
        { Math::cos(theta), 0.f, -Math::sin(theta), 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { Math::sin(theta), 0.f, Math::cos(theta), 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createRotationZ(float theta) {
    float temp[4][4] = {
        { Math::cos(theta), Math::sin(theta), 0.f, 0.f },
        { -Math::sin(theta), Math::cos(theta), 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createFromQuaternion(const Quaternion& q) {
    float mat[4][4];

    mat[0][0] = 1.f - 2.f * q.y * q.y - 2.f * q.z * q.z;
    mat[0][1] = 2.f * q.x * q.y + 2.f * q.w * q.z;
    mat[0][2] = 2.f * q.x * q.z - 2.f * q.w * q.y;
    mat[0][3] = 0.f;

    mat[1][0] = 2.f * q.x * q.y - 2.f * q.w * q.z;
    mat[1][1] = 1.f - 2.f * q.x * q.x - 2.f * q.z * q.z;
    mat[1][2] = 2.f * q.y * q.z + 2.f * q.w * q.x;
    mat[1][3] = 0.f;

    mat[2][0] = 2.f * q.x * q.z + 2.f * q.w * q.y;
    mat[2][1] = 2.f * q.y * q.z - 2.f * q.w * q.x;
    mat[2][2] = 1.f - 2.f * q.x * q.x - 2.f * q.y * q.y;
    mat[2][3] = 0.f;

    mat[3][0] = 0.f;
    mat[3][1] = 0.f;
    mat[3][2] = 0.f;
    mat[3][3] = 1.f;

    return Matrix4(mat);
}

Matrix4 Matrix4::createTranslation(const Vector3& trans) {
    float temp[4][4] = {
        { 1.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { trans.x, trans.y, trans.z, 1.f }
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createOrtho(float width, float height, float _near, float _far) {
    float temp[4][4] = {
        { 2.f / width, 0.f, 0.f, 0.f },
        { 0.f, -2.f / height, 0.f, 0.f },
        { 0.f, 0.f, 1.f / (_far - _near), 0.f },
        { -1.f, 1.f, _near / (_near - _far), 1.f }
    };
    return Matrix4(temp);
}

float m4Ident[4][4] = {
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f }
};
const Matrix4 Matrix4::identity(m4Ident);
