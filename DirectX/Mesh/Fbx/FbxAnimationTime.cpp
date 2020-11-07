#include "FbxAnimationTime.h"

FbxAnimationTime::FbxAnimationTime() :
    mPeriod(),
    mStart(),
    mStop(),
    mStartFrame(0),
    mStopFrame(0),
    mFramePerSec(0) {
}

FbxAnimationTime::~FbxAnimationTime() = default;

void FbxAnimationTime::parse(FbxScene* fbxScene) {
    //テイクから基準時間を取得
    FbxArray<FbxString*> takeNames;
    fbxScene->FillAnimStackNameArray(takeNames);
    int numTake = takeNames.GetCount();
    bool isTakeExist = false;
    for (int i = 0; i < numTake; ++i) {
        FbxTakeInfo* currentTakeInfo = fbxScene->GetTakeInfo(*takeNames[i]);
        if (currentTakeInfo) {
            mStart = currentTakeInfo->mLocalTimeSpan.GetStart();
            mStop = currentTakeInfo->mLocalTimeSpan.GetStop();
            isTakeExist = true;
            break;
        }
    }

    //テイクが確認できなかったらアニメーション無し
    if (!isTakeExist) {
        mStart = 0;
        mStop = 0;
    }

    //時間モードから単位時間を算出
    FbxGlobalSettings& globalSettings = fbxScene->GetGlobalSettings();
    FbxTime::EMode timeMode = globalSettings.GetTimeMode();
    mPeriod.SetTime(0, 0, 0, 1, 0, timeMode);

    mStartFrame = static_cast<int>(mStart.Get() / mPeriod.Get());
    mStopFrame = static_cast<int>(mStop.Get() / mPeriod.Get());

    //1フレームの時間を算出
    FbxTime framePerSecTime;
    framePerSecTime.SetTime(0, 0, 1, 0, 0, timeMode);
    mFramePerSec = static_cast<int>(framePerSecTime.Get() / mPeriod.Get());

    //FbxArrayを削除する
    FbxArrayDelete(takeNames);
}

int FbxAnimationTime::getStopFrame() const {
    return mStopFrame;
}

FbxTime FbxAnimationTime::getTime(int frame) const {
    return mStart + mPeriod * frame;
}
