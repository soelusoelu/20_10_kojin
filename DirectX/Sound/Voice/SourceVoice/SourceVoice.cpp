#include "SourceVoice.h"
#include "../../3D/Emitter/Sound3DEmitter.h"
#include "../../Effects/SoundEffect.h"
#include "../../Effects/SoundEffectCollection.h"
#include "../../Player/SoundPlayer.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Volume/SoundVolume.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mDetails{ format.channels, format.samplesPerSec },
    mSoundData(std::make_unique<SoundData>(*loader, format)),
    mSoundVolume(std::make_unique<SoundVolume>(*this, format.channels, masteringVoice.getVoiceDetails().channels)),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, loader, format, param.maxFrequencyRatio)),
    mEmitter(nullptr) {
    //3D演算をするなら
    if (param.isCalculate3D) {
        mEmitter = std::make_unique<Sound3DEmitter>(*this, masteringVoice, format);
    }
    //再生時間設定/取得のためにすべてのソースボイスでエフェクト付与
    mSoundEffect->getEffectCollection().getPlayTimer();
}

SourceVoice::~SourceVoice() {
    mXAudio2SourceVoice->DestroyVoice();
    mXAudio2SourceVoice = nullptr;
}

IXAudio2Voice* SourceVoice::getXAudio2Voice() const {
    return mXAudio2SourceVoice;
}

const VoiceDetails& SourceVoice::getVoiceDetails() const {
    return mDetails;
}

SoundVolume& SourceVoice::getSoundVolume() const {
    return *mSoundVolume;
}

OutputVoices& SourceVoice::getOutputVoices() const {
    return *mOutputVoices;
}

SoundEffect& SourceVoice::getSoundEffect() const {
    return *mSoundEffect;
}

void SourceVoice::update() {
    mSoundVolume->update();
    mSoundPlayer->update();
    if (mEmitter) {
        mEmitter->update();
    }
}

IXAudio2SourceVoice* SourceVoice::getXAudio2SourceVoice() const {
    return mXAudio2SourceVoice;
}

SoundData& SourceVoice::getSoundData() const {
    return *mSoundData;
}

SoundPlayer& SourceVoice::getSoundPlayer() const {
    return *mSoundPlayer;
}

Sound3DEmitter& SourceVoice::getEmitter() const {
    return *mEmitter;
}

bool SourceVoice::isCalculate3D() const {
    return (mEmitter) ? true : false;
}
