#pragma once

class Vector2;

class Matrix3 {
public:
    float m[3][3];

public:
    Matrix3();

    explicit Matrix3(float inMat[3][3]);

    // Matrix multiplication
    friend Matrix3 operator*(const Matrix3& left, const Matrix3& right);

    Matrix3& operator*=(const Matrix3& right);

    // Create a scale matrix with x and y scales
    static Matrix3 createScale(float xScale, float yScale);

    static Matrix3 createScale(const Vector2& scaleVector);

    // Create a scale matrix with a uniform factor
    static Matrix3 createScale(float scale);

    // Create a rotation matrix about the Z axis
    // theta is in radians
    static Matrix3 createRotation(float theta);

    // Create a translation matrix (on the xy-plane)
    static Matrix3 createTranslation(const Vector2& trans);

    static const Matrix3 identity;
};
