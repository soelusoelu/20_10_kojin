#include "SimpleReverb.h"
#include "../../../DebugLayer/Debug.h"

SimpleReverb::SimpleReverb() :
    mXAPO(nullptr) {
}

SimpleReverb::~SimpleReverb() = default;

bool SimpleReverb::create(XAUDIO2_EFFECT_DESCRIPTOR* desc) {
    auto res = CreateFX(__uuidof(FXReverb), &mXAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created equalizer.");
        return false;
    }

    desc->pEffect = mXAPO;

    return true;
}
