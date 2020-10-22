#pragma once

#include "../Math/Math.h"

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
    Vector3 tangent;
};

struct PosNormUVVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};