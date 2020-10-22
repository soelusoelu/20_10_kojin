#include "Frequency.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"

Frequency::Frequency(SourceVoice& sourceVoice, SoundPlayer& player, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mPlayer(player),
    mCurrentFrequencyRatio(1.f),
    mMaxFrequencyRatio(maxFrequencyRatio) {
}

Frequency::~Frequency() = default;

void Frequency::setFrequencyRatio(float pitch) {
    mCurrentFrequencyRatio = pitch;

    if (mCurrentFrequencyRatio > mMaxFrequencyRatio) {
        mCurrentFrequencyRatio = mMaxFrequencyRatio;
        Debug::logWarning("Pitch is over max frequency.");
    }

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFrequencyRatio(mCurrentFrequencyRatio);
    //再生時間にも影響するので
    mPlayer.getPlayTimer().setFrequency(mCurrentFrequencyRatio);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed change frequency ratio.");
    }
#endif // _DEBUG
}

float Frequency::getFrequencyRatio() const {
    return mCurrentFrequencyRatio;
}
