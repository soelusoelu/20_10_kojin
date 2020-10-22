#include "SubmixVoice.h"
#include "../../Effects/SoundEffect.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Volume/SoundVolume.h"

SubmixVoice::SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param) :
    mXAudio2SubmixVoice(XAudio2SubmixVoice),
    mDetails{ param.channels, param.inputSampleRate },
    mSoundVolume(std::make_unique<SoundVolume>(*this, param.channels, masteringVoice.getVoiceDetails().channels)),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this)) {
}

SubmixVoice::~SubmixVoice() {
    mXAudio2SubmixVoice->DestroyVoice();
    mXAudio2SubmixVoice = nullptr;
}

IXAudio2Voice* SubmixVoice::getXAudio2Voice() const {
    return mXAudio2SubmixVoice;
}

const VoiceDetails& SubmixVoice::getVoiceDetails() const {
    return mDetails;
}

SoundVolume& SubmixVoice::getSoundVolume() const {
    return *mSoundVolume;
}

OutputVoices& SubmixVoice::getOutputVoices() const {
    return *mOutputVoices;
}

SoundEffect& SubmixVoice::getSoundEffect() const {
    return *mSoundEffect;
}
