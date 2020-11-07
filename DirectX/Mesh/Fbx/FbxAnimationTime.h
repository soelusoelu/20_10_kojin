#pragma once

#include <fbxsdk.h>

//FBXアニメーションの時間を扱うクラス
class FbxAnimationTime {
public:
    FbxAnimationTime();
    ~FbxAnimationTime();

    //解析
    void parse(FbxScene* fbxScene);
    //アニメーション終了フレームを取得する
    int getStopFrame() const;
    //指定フレームの時間を取得する
    FbxTime getTime(int frame) const;

private:
    //単位時間
    FbxTime mPeriod;
    //スタート時間
    FbxTime mStart;
    //ストップ時間
    FbxTime mStop;
    //スタートフレーム
    int mStartFrame;
    //ストップフレーム
    int mStopFrame;
    //1秒あたりのフレーム数
    int mFramePerSec;
};
