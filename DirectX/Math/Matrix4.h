#pragma once

class Vector3;
class Quaternion;

class Matrix4 {
public:
    float m[4][4];

public:
    Matrix4();

    explicit Matrix4(float inMat[4][4]);

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);

    Matrix4& operator*=(const Matrix4& right);

    void transpose();

    // Invert the matrix - super slow
    void inverse();

    static Matrix4 inverse(const Matrix4& right);

    // Get the translation component of the matrix
    Vector3 getTranslation() const;

    // Get the X axis of the matrix (forward)
    Vector3 getXAxis() const;

    // Get the Y axis of the matrix (left)
    Vector3 getYAxis() const;

    // Get the Z axis of the matrix (up)
    Vector3 getZAxis() const;

    // Extract the scale component from the matrix
    Vector3 getScale() const;

    // Create a scale matrix with x, y, and z scales
    static Matrix4 createScale(float xScale, float yScale, float zScale);

    static Matrix4 createScale(const Vector3& scaleVector);

    // Create a scale matrix with a uniform factor
    static Matrix4 createScale(float scale);

    // Rotation about x-axis
    static Matrix4 createRotationX(float theta);

    // Rotation about y-axis
    static Matrix4 createRotationY(float theta);

    // Rotation about z-axis
    static Matrix4 createRotationZ(float theta);

    // Create a rotation matrix from a quaternion
    static Matrix4 createFromQuaternion(const Quaternion& q);

    static Matrix4 createTranslation(const Vector3& trans);

    static Matrix4 createOrtho(float width, float height, float _near, float _far);

    static const Matrix4 identity;
};
