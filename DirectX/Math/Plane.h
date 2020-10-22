#pragma once

class Vector3;

class Plane {
public:
    float a;
    float b;
    float c;
    float d;

public:
    //aX + bY + cZ = d
    Plane(float inA, float inB, float inC, float inD);
    //法線ベクトル(a, b, c)と平面の距離(D = d / |n|)
    Plane(const Vector3& normal, float d);
    //3点を通る面
    Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
    //法線ベクトル(a, b, c)と任意の点
    Plane(const Vector3& normal, const Vector3& p);
    //法線ベクトルの取得(正規化はしていない)
    Vector3 normal() const;
    //指定ベクトルと法線ベクトルの内積を計算
    float dotNormal(const Vector3& vector) const;
    //平面要素を正規化
    void normalize();
};
