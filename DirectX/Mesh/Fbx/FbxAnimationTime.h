#pragma once

#include "FbxMotionTime.h"
#include "../Motion.h"
#include <fbxsdk.h>
#include <vector>

//FBXアニメーションの時間を扱うクラス
class FbxAnimationTime {
public:
    FbxAnimationTime();
    ~FbxAnimationTime();

    //解析
    void parse(
        std::vector<Motion>& motions,
        std::vector<FbxMotionTime>& motionsTime,
        FbxScene* fbxScene
    );

private:
    void calculateMotionTime(
        Motion& motion,
        FbxMotionTime& motionTime,
        FbxScene* fbxScene,
        const FbxTakeInfo* fbxTakeInfo
    );
};
