#pragma once

#include "Bone.h"
#include <string>
#include <vector>

//モーション構造体
struct Motion {
    //モーション名
    std::string name;
    //フレーム時姿勢
    //ボーン数 * フレーム数
    std::vector<std::vector<Matrix4>> frameMat;
    //スタートフレーム
    int startFrame;
    //フレーム数
    int numFrame;
};
