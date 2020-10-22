#pragma once

#include "../ISoundEffect.h"
#include <xaudio2.h>
#include <xaudio2fx.h>

#pragma comment(lib, "xaudio2.lib")

//リバーブ
class Reverb : public ISoundEffect {
public:
    Reverb();
    ~Reverb();
     virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) override;
     static XAUDIO2FX_REVERB_PARAMETERS getParameters();

private:
    Reverb(const Reverb&) = delete;
    Reverb& operator=(const Reverb&) = delete;

private:
    IUnknown* mXAPO;
};
