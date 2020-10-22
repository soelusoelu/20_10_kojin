#include "Reverb.h"
#include "../../../DebugLayer/Debug.h"

Reverb::Reverb() :
    mXAPO(nullptr) {
}

Reverb::~Reverb() = default;

bool Reverb::create(XAUDIO2_EFFECT_DESCRIPTOR* desc) {
    auto res = XAudio2CreateReverb(&mXAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created reverb.");
        return false;
    }

    desc->pEffect = mXAPO;

    return true;
}

XAUDIO2FX_REVERB_PARAMETERS Reverb::getParameters() {
    XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Param = XAUDIO2FX_I3DL2_PRESET_BATHROOM;
    XAUDIO2FX_REVERB_PARAMETERS reverbParam;

    ReverbConvertI3DL2ToNative(&i3dl2Param, &reverbParam);

    return reverbParam;
}
