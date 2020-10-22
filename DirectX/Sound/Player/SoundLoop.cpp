#include "SoundLoop.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Data/SoundData.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

SoundLoop::SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player) :
    mPlayer(player),
    mSourceVoice(sourceVoice),
    mLoopBegin(0.f),
    mLoopEnd(0.f),
    mIsLoop(false) {
}

SoundLoop::~SoundLoop() = default;

void SoundLoop::update() {
    if (!mIsLoop) {
        return;
    }

    //現在の再生時間がループ折返し地点を超えていたらループの開始地点に戻す
    if (mPlayer.getPlayTimer().getPlayTime() >= mLoopEnd - 0.01f) {
        mPlayer.setPlayPoint(mLoopBegin);
        return;
    }

    //上記の関数だと誤差で終端ループできないときがある
    //キューが空っぽなら
    XAUDIO2_VOICE_STATE state = { 0 };
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    if (state.BuffersQueued == 0) {
        //曲の最後でループするか
        if (Math::equal(mLoopEnd, mSourceVoice.getSoundData().length())) {
            mPlayer.setPlayPoint(mLoopBegin);
        }
    }
}

void SoundLoop::setLoopPoint(float begin, float end) {
    const auto& data = mSourceVoice.getSoundData();

    //ループの開始が曲の長さを超えてたら
    if (begin >= data.length()) {
        Debug::logWarning("The loop start exceeds the length of the song.");
        return;
    }

    //折返しの値が0なら曲の終わりまで
    if (Math::nearZero(end)) {
        end = data.length();
    }

    setValue(begin, end);
    clamp();
}

void SoundLoop::loopAll() {
    setValue(0.f, mSourceVoice.getSoundData().length());
}

void SoundLoop::exitLoop() {
    mIsLoop = false;
}

bool SoundLoop::isLoop() const {
    return mIsLoop;
}

float SoundLoop::getLoopEnd() const {
    return mLoopEnd;
}

void SoundLoop::setValue(float begin, float end) {
    mLoopBegin = begin;
    mLoopEnd = end;
    mIsLoop = true;
}

void SoundLoop::clamp() {
    const auto& data = mSourceVoice.getSoundData();
    mLoopBegin = data.clamp(mLoopBegin);
    mLoopEnd = data.clamp(mLoopEnd);
}
