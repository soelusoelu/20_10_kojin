#pragma once

#include "../Math/Math.h"

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;

    //スキンメッシュ用
    float weight[4];
    unsigned short index[4];

    MeshVertex() :
        pos(Vector3::zero),
        normal(Vector3::zero),
        uv(Vector2::zero),
        weight{ 0.f, 0.f, 0.f, 0.f },
        index{ 0, 0, 0, 0 } {
    }
};

//struct MeshVertex {
//    Vector3 pos;
//    Vector3 normal;
//    Vector2 uv;
//    Vector3 tangent;
//};

struct PosNormUVVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};
