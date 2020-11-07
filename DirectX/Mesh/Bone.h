#pragma once

#include "../Math/Math.h"
#include <string>
#include <vector>

//ボーン構造体
struct Bone {
    //ボーン名
    std::string name;
    //フレーム数
    unsigned numFrame;
    //初期姿勢
    Matrix4 initMat;
    //オフセット行列
    Matrix4 offsetMat;
    //フレーム時姿勢
    std::vector<Matrix4> frameMat;

    //親ノード
    Bone* parent;
    //小ボーンたち
    std::vector<Bone*> children;
};
