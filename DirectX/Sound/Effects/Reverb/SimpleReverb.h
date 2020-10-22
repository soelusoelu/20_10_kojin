#pragma once

#include "../ISoundEffect.h"
#include <xapofx.h>

//簡易版リバーブ
class SimpleReverb : public ISoundEffect {
public:
    SimpleReverb();
    ~SimpleReverb();
    virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) override;

private:
    SimpleReverb(const SimpleReverb&) = delete;
    SimpleReverb& operator=(const SimpleReverb&) = delete;

private:
    IUnknown* mXAPO;
};
