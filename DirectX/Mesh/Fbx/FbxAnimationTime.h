#pragma once

#include "FbxMotionTime.h"
#include <fbxsdk.h>

//FBXアニメーションの時間を扱うクラス
class FbxAnimationTime {
public:
    FbxAnimationTime();
    ~FbxAnimationTime();

    //モーションの時間管理クラスを取得する
    FbxMotionTime getMotionTime(
        FbxScene* fbxScene,
        const FbxAnimStack* fbxAnimStack
    ) const;

    void getMotionTime(
        FbxMotionTime& motionTime,
        FbxScene* fbxScene,
        const FbxAnimStack* fbxAnimStack
    ) const;
};
