#include "SoundFade.h"
#include "SoundVolume.h"
#include "../../Device/Time.h"
#include "../../Math/Math.h"

SoundFade::SoundFade(SoundVolume& soundVolume) :
    mSoundVolume(soundVolume),
    mFadeEndFunc(nullptr),
    mTargetVolume(0.f),
    mTargetTime(0.f),
    mBeforeVolume(0.f),
    mTimeRate(0.f),
    mIsFading(false) {
}

SoundFade::~SoundFade() = default;

void SoundFade::settings(float targetVolume, float targetTime, const std::function<void()>& f) {
    //targetTimeがほぼ0(フェードする必要がない)の場合
    if (Math::nearZero(targetTime)) {
        mSoundVolume.setVolume(targetVolume);
        mIsFading = false;
        if (f) {
            f();
        }
        return;
    }

    mTargetVolume = targetVolume;
    mTargetTime = targetTime;
    mFadeEndFunc = f;
    mBeforeVolume = mSoundVolume.getVolume();
    mTimeRate = 0.f;
    mIsFading = true;
}

void SoundFade::update() {
    if (mIsFading) {
        updateFade();
    }
}

bool SoundFade::isFading() const {
    return mIsFading;
}

void SoundFade::updateFade() {
    updateLerpTimer();
    nextVolumeFading();
    fadeEnd();
}

void SoundFade::updateLerpTimer() {
    mTimeRate += Time::deltaTime / mTargetTime;
    if (mTimeRate >= 1.f) {
        mTimeRate = 1.f;
        mIsFading = false;
    }
}

void SoundFade::nextVolumeFading() {
    auto nextVolume = Math::lerp(mBeforeVolume, mTargetVolume, mTimeRate);
    mSoundVolume.setVolume(nextVolume);
}

void SoundFade::fadeEnd() {
    if (!mIsFading) {
        if (mFadeEndFunc) {
            mFadeEndFunc();
        }
    }
}
