#pragma once

#include <fbxsdk.h>

//モーションの時間構造体
struct FbxMotionTime {
    //単位時間
    FbxTime period;
    //スタート時間
    FbxTime start;
    //ストップ時間
    FbxTime stop;
    //スタートフレーム
    int startFrame;
    //ストップフレーム
    int stopFrame;
    //1フレームあたりの秒数
    float framePerSec;

    //指定フレームの時間を取得する
    FbxTime getTime(int frame) const {
        return start + period * frame;
    }
};
