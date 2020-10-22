#include "Equalizer.h"
#include "../../../DebugLayer/Debug.h"

Equalizer::Equalizer() :
    mXAPO(nullptr) {
}

Equalizer::~Equalizer() = default;

bool Equalizer::create(XAUDIO2_EFFECT_DESCRIPTOR* desc) {
    auto res = CreateFX(__uuidof(FXEQ), &mXAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created equalizer.");
        return false;
    }

    desc->pEffect = mXAPO;

    return true;
}
