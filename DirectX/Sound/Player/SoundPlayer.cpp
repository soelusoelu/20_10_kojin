#include "SoundPlayer.h"
#include "Frequency.h"
#include "SoundLoop.h"
#include "SoundPlayTimer.h"
#include "../Data/SoundData.h"
#include "../Streaming/SoundStreaming.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundFade.h"
#include "../Volume/SoundVolume.h"
#include "../../DebugLayer/Debug.h"

SoundPlayer::SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mStreaming(std::make_unique<SoundStreaming>(sourceVoice, *this, loader, format)),
    mPlayTimer(std::make_unique<SoundPlayTimer>(sourceVoice, *this)),
    mLoop(std::make_unique<SoundLoop>(sourceVoice, *this)),
    mFrequency(std::make_unique<Frequency>(sourceVoice, *this, maxFrequencyRatio)),
    mIsPlay(false) {
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::update() {
    if (!mIsPlay) {
        return;
    }

    mLoop->update();
    mStreaming->polling();

    //キューが空っぽなら
    XAUDIO2_VOICE_STATE state = { 0 };
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    if (state.BuffersQueued == 0) {
        //曲の最初に戻る
        mStreaming->seek(0.f);
        pause();
    }
}

void SoundPlayer::playStreaming() {
    mIsPlay = true;
    mStreaming->polling();
    mPlayTimer->startTimer();
    auto res = mSourceVoice.getXAudio2SourceVoice()->Start();
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed play streaming.");
    }
#endif // _DEBUG
}

void SoundPlayer::playStreamingFadeIn(float targetVolume, float targetTime) {
    mSourceVoice.getSoundVolume().setVolume(0.f);
    mSourceVoice.getSoundVolume().getFade().settings(targetVolume, targetTime);
    playStreaming();
}

void SoundPlayer::setPlayPoint(float point) {
    //適正範囲にクランプ
    float sec = mSourceVoice.getSoundData().clamp(point);

    mPlayTimer->setPlayTime(sec);
    mStreaming->seek(sec);
}

void SoundPlayer::pause() {
    bool res = pauseAndStop(XAUDIO2_PLAY_TAILS);
#ifdef _DEBUG
    if (!res) {
        Debug::logError("Failed sound pause.");
    }
#endif // _DEBUG
}

void SoundPlayer::pauseFadeOut(float targetTime) {
    mSourceVoice.getSoundVolume().getFade().settings(0.f, targetTime, [&]() { pause(); });
}

void SoundPlayer::stop() {
    bool res = pauseAndStop(0);
#ifdef _DEBUG
    if (!res) {
        Debug::logError("Failed sound stop.");
    }
#endif // _DEBUG
}

void SoundPlayer::stopFadeOut(float targetTime) {
    mSourceVoice.getSoundVolume().getFade().settings(0.f, targetTime, [&]() { stop(); });
}

bool SoundPlayer::isStop() const {
    return (!mIsPlay);
}

SoundPlayTimer& SoundPlayer::getPlayTimer() const {
    return *mPlayTimer;
}

SoundLoop& SoundPlayer::getLoop() const {
    return *mLoop;
}

Frequency& SoundPlayer::getFrequency() const {
    return *mFrequency;
}

bool SoundPlayer::pauseAndStop(unsigned flag) {
    mIsPlay = false;
    mPlayTimer->stopTimer();
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flag);
    return (SUCCEEDED(res));
}
