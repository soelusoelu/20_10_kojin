#include "SoundPlayTimer.h"
#include "Frequency.h"
#include "SoundPlayer.h"
#include "../Effects/SoundEffect.h"
#include "../Effects/SoundEffectCollection.h"
#include "../Effects/PlayTimer/PlayTimerParam.h"
#include "../Voice/SourceVoice/SourceVoice.h"

SoundPlayTimer::SoundPlayTimer(SourceVoice& sourceVoice, SoundPlayer& player) :
    mSourceVoice(sourceVoice),
    mPlayer(player) {
}

SoundPlayTimer::~SoundPlayTimer() = default;

void SoundPlayTimer::setPlayTime(float time) {
    PlayTimerParam param;
    param.setTime = time;
    param.frequencyRatio = mPlayer.getFrequency().getFrequencyRatio();
    mSourceVoice.getSoundEffect().setEffectParameters(SoundEffectCollection::PLAY_TIMER_ID, &param, sizeof(PlayTimerParam));
}

void SoundPlayTimer::setFrequency(float getFrequency) {
    PlayTimerParam param;
    param.setTime = getPlayTime();
    param.frequencyRatio = getFrequency;
    mSourceVoice.getSoundEffect().setEffectParameters(SoundEffectCollection::PLAY_TIMER_ID, &param, sizeof(PlayTimerParam));
}

float SoundPlayTimer::getPlayTime() const {
    float out = 0.f;
    mSourceVoice.getSoundEffect().getEffectParameters(SoundEffectCollection::PLAY_TIMER_ID, &out, sizeof(out));
    return out;
}

void SoundPlayTimer::startTimer() {
    mSourceVoice.getSoundEffect().enable(SoundEffectCollection::PLAY_TIMER_ID);
}

void SoundPlayTimer::stopTimer() {
    mSourceVoice.getSoundEffect().disable(SoundEffectCollection::PLAY_TIMER_ID);
}
