#pragma once

#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

//3Dサウンドで扱うリスナー構造体
struct Sound3DListenerParam {
    //前方ベクトル
    Vector3 orientFront;
    //上ベクトル
    //orientFrontと直行している必要がある
    Vector3 orientTop;
    //リスナーの位置
    Vector3 position;
    //ドップラー計算に使用される速度ベクトル
    Vector3 velocity;
    //指向性の指定
    //NULLだと無指向(コーン処理が適用されない)
    Sound3DCone* cone;

    Sound3DListenerParam() :
        orientFront(Vector3::forward),
        orientTop(Vector3::up),
        position(Vector3::zero),
        velocity(Vector3::zero),
        cone(nullptr) {
    }
};
