#pragma once

#include "SubmixVoiceInitParam.h"
#include "../IVoice.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;

//IXAudio2SubmixVoiceラッパークラス
class SubmixVoice : public IVoice {
public:
    SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param);
    ~SubmixVoice();

    virtual IXAudio2Voice* getXAudio2Voice() const override;
    virtual const VoiceDetails& getVoiceDetails() const override;
    virtual SoundVolume& getSoundVolume() const override;
    virtual OutputVoices& getOutputVoices() const override;
    virtual SoundEffect& getSoundEffect() const override;

private:
    SubmixVoice(const SubmixVoice&) = delete;
    SubmixVoice& operator=(const SubmixVoice&) = delete;

private:
    IXAudio2SubmixVoice* mXAudio2SubmixVoice;
    VoiceDetails mDetails;
    std::unique_ptr<SoundVolume> mSoundVolume;
    std::unique_ptr<OutputVoices> mOutputVoices;
    std::unique_ptr<SoundEffect> mSoundEffect;
};
