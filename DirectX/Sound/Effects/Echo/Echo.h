#pragma once

#include "../ISoundEffect.h"
#include <xapofx.h>

//エコー
class Echo : public ISoundEffect {
public:
    Echo();
    ~Echo();
    virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) override;

private:
    Echo(const Echo&) = delete;
    Echo& operator=(const Echo&) = delete;

private:
    IUnknown* mXAPO;
};
