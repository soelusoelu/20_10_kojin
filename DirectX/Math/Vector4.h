#pragma once

class Vector3;

class Vector4 {
public:
    float x;
    float y;
    float z;
    float w;

public:
    Vector4();

    explicit Vector4(float inX, float inY, float inZ, float inW);

    Vector4(const Vector3& vec3, float inW);

    Vector4& operator=(const Vector4& vec);
};
