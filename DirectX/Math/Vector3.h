#pragma once

class Vector2;
class Matrix4;
class Quaternion;

class Vector3 {
public:
    float x;
    float y;
    float z;

public:
    Vector3();

    explicit Vector3(float inX, float inY, float inZ);

    Vector3(const Vector2& vec2, float inZ);

    // Set all three components in one line
    void set(float inX, float inY, float inZ);

    Vector3& operator=(const Vector3& vec);

    Vector3 operator-();

    // Vector addition (a + b)
    friend Vector3 operator+(const Vector3& a, const Vector3& b);

    // Vector subtraction (a - b)
    friend Vector3 operator-(const Vector3& a, const Vector3& b);

    // Component-wise multiplication
    friend Vector3 operator*(const Vector3& left, const Vector3& right);

    // Scalar multiplication
    friend Vector3 operator*(const Vector3& vec, float scalar);

    // Scalar multiplication
    friend Vector3 operator*(float scalar, const Vector3& vec);

    friend Vector3 operator/(const Vector3& vec, float scalar);

    // Scalar *=
    Vector3& operator*=(float scalar);

    Vector3& operator*=(const Vector3& right);

    // Vector +=
    Vector3& operator+=(const Vector3& right);

    // Vector -=
    Vector3& operator-=(const Vector3& right);

    // Length squared of vector
    float lengthSq() const;

    // Length of vector
    float length() const;

    static float distance(const Vector3& a, const Vector3& b);

    void clamp(const Vector3& min, const Vector3& max);

    static Vector3 clamp(const Vector3& value, const Vector3& min, const Vector3& max);

    // Normalize this vector
    void normalize();

    // Normalize the provided vector
    static Vector3 normalize(const Vector3& vec);

    // Dot product between two vectors (a dot b)
    static float dot(const Vector3& a, const Vector3& b);

    // Cross product between two vectors (a cross b)
    static Vector3 cross(const Vector3& a, const Vector3& b);

    // Lerp from A to B by f
    static Vector3 lerp(const Vector3& a, const Vector3& b, float f);

    // Reflect V about (normalized) N
    static Vector3 reflect(const Vector3& v, const Vector3& n);

    static Vector3 transform(const Vector3& vec, const Matrix4& mat, float w = 1.0f);
    // This will transform the vector and renormalize the w component
    static Vector3 transformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w = 1.0f);

    // Transform a Vector3 by a quaternion
    static Vector3 transform(const Vector3& v, const Quaternion& q);

    static const Vector3 zero;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 down;
    static const Vector3 back;
    static const Vector3 one;
    static const Vector3 negOne;
    static const Vector3 infinity;
    static const Vector3 negInfinity;
};
