#pragma once

#include "VoiceDetails.h"
#include <xaudio2.h>

class SoundVolume;
class OutputVoices;
class SoundEffect;

//ボイスインターフェース
class IVoice {
public:
    virtual ~IVoice() = default;
    virtual IXAudio2Voice* getXAudio2Voice() const = 0;
    virtual const VoiceDetails& getVoiceDetails() const = 0;
    virtual SoundVolume& getSoundVolume() const = 0;
    virtual OutputVoices& getOutputVoices() const = 0;
    virtual SoundEffect& getSoundEffect() const = 0;
};
