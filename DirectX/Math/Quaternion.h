#pragma once

class Vector3;

class Quaternion {
public:
    float x;
    float y;
    float z;
    float w;

public:
    Quaternion();

    explicit Quaternion(float inX, float inY, float inZ, float inW);
    explicit Quaternion(const Vector3& axis, float angle);
    explicit Quaternion(const Vector3& euler);

    // Directly set the internal components
    void set(float inX, float inY, float inZ, float inW);

    void conjugate();

    float lengthSq() const;

    float length() const;

    void normalize();

    //オイラー角をもとにクォータニオンを設定する
    void setEuler(const Vector3& euler);

    //クォータニオンからオイラー角を取得する
    Vector3 euler() const;

    // Normalize the provided quaternion
    static Quaternion normalize(const Quaternion& q);

    // Linear interpolation
    static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f);

    static float dot(const Quaternion& a, const Quaternion& b);

    // Spherical Linear Interpolation
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f);

    // Concatenate
    // Rotate by q FOLLOWED BY p
    //掛け算
    static Quaternion concatenate(const Quaternion& q, const Quaternion& p);

    static const Quaternion identity;
};
