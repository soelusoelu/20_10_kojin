#include "FbxAnimationTime.h"

FbxAnimationTime::FbxAnimationTime() = default;

FbxAnimationTime::~FbxAnimationTime() = default;

FbxMotionTime FbxAnimationTime::getMotionTime(
    FbxScene * fbxScene,
    const FbxAnimStack * fbxAnimStack
) const {
    FbxMotionTime motionTime;

    getMotionTime(motionTime, fbxScene, fbxAnimStack);

    return motionTime;
}

void FbxAnimationTime::getMotionTime(
    FbxMotionTime& motionTime,
    FbxScene* fbxScene,
    const FbxAnimStack* fbxAnimStack
) const {
    motionTime.start = fbxAnimStack->LocalStart.Get();
    motionTime.stop = fbxAnimStack->LocalStop.Get();

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
}
