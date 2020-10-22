#pragma once

class Matrix3;

class Vector2 {
public:
    float x;
    float y;

public:
    Vector2();

    explicit Vector2(float inX, float inY);

    // Set both components in one line
    void set(float inX, float inY);

    Vector2 operator-();

    friend Vector2 operator-(const Vector2& vec);

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b);

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b);

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b);

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, float scalar);

    // Scalar multiplication
    friend Vector2 operator*(float scalar, const Vector2& vec);

    friend Vector2 operator/(const Vector2& vec, float scalar);

    // Scalar *=
    Vector2& operator*=(float scalar);

    // Vector +=
    Vector2& operator+=(const Vector2& right);

    // Vector -=
    Vector2& operator-=(const Vector2& right);

    // Length squared of vector
    float lengthSq() const;

    // Length of vector
    float length() const;

    static float distance(const Vector2& a, const Vector2& b);

    void clamp(const Vector2& min, const Vector2& max);

    static Vector2 clamp(const Vector2& value, const Vector2& min, const Vector2& max);

    // Normalize this vector
    void normalize();

    // Normalize the provided vector
    static Vector2 normalize(const Vector2& vec);

    // Dot product between two vectors (a dot b)
    static float dot(const Vector2& a, const Vector2& b);

    static float cross(const Vector2& a, const Vector2& b);

    // Lerp from A to B by f
    static Vector2 lerp(const Vector2& a, const Vector2& b, float f);

    // Reflect V about (normalized) N
    static Vector2 reflect(const Vector2& v, const Vector2& n);

    // Transform vector by matrix
    static Vector2 transform(const Vector2& vec, const Matrix3& mat, float w = 1.0f);

    static const Vector2 zero;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 left;
    static const Vector2 down;
    static const Vector2 one;
};
