#include "FbxAnimationTime.h"

FbxAnimationTime::FbxAnimationTime() = default;

FbxAnimationTime::~FbxAnimationTime() = default;

void FbxAnimationTime::parse(
    std::vector<Motion>& motions,
    std::vector<FbxMotionTime>& motionsTime,
    FbxScene* fbxScene
) {
    //テイクから基準時間を取得
    FbxArray<FbxString*> takeNames;
    fbxScene->FillAnimStackNameArray(takeNames);

    //モーション数を設定する
    motions.resize(takeNames.GetCount());
    motionsTime.resize(motions.size());

    //モーションの数だけ時間を取得する
    for (int i = 0; i < motions.size(); ++i) {
        motions[i].name = static_cast<std::string>(*takeNames[i]);
        FbxTakeInfo* currentTakeInfo = fbxScene->GetTakeInfo(*takeNames[i]);
        if (currentTakeInfo) {
            //モーション時間を計算し取得する
            calculateMotionTime(motions[i], motionsTime[i], fbxScene, currentTakeInfo);
        }
    }

    //FbxArrayを削除する
    FbxArrayDelete(takeNames);
}

void FbxAnimationTime::calculateMotionTime(
    Motion& motion,
    FbxMotionTime& motionTime,
    FbxScene* fbxScene,
    const FbxTakeInfo* fbxTakeInfo
) {
    motionTime.start = fbxTakeInfo->mLocalTimeSpan.GetStart();
    motionTime.stop = fbxTakeInfo->mLocalTimeSpan.GetStop();

    //時間モードから単位時間を算出
    FbxGlobalSettings& globalSettings = fbxScene->GetGlobalSettings();
    FbxTime::EMode timeMode = globalSettings.GetTimeMode();
    motionTime.period.SetTime(0, 0, 0, 1, 0, timeMode);

    motionTime.startFrame = static_cast<int>(motionTime.start.Get() / motionTime.period.Get());
    motionTime.stopFrame = static_cast<int>(motionTime.stop.Get() / motionTime.period.Get());

    //1フレームの時間を算出
    FbxTime framePerSecTime;
    framePerSecTime.SetTime(0, 0, 1, 0, 0, timeMode);
    motionTime.framePerSec = static_cast<float>(framePerSecTime.Get() / motionTime.period.Get());

    //モーションのフレーム数を設定
    motion.numFrame = motionTime.stopFrame - motionTime.startFrame;
}
