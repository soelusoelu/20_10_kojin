#include "OutputVoices.h"
#include "../../../DebugLayer/Debug.h"
#include <cassert>

OutputVoices::OutputVoices(IVoice& voice) :
    mVoice(voice) {
}

OutputVoices::~OutputVoices() = default;

void OutputVoices::addOutputVoice(const IVoice& voice, bool useFilter, bool isApply) {
    XAUDIO2_SEND_DESCRIPTOR desc = { 0 };
    desc.Flags = (useFilter) ? XAUDIO2_SEND_USEFILTER : 0;
    desc.pOutputVoice = voice.getXAudio2Voice();
    mDescs.emplace_back(desc);

    if (isApply) {
        apply();
    }
}

const XAUDIO2_SEND_DESCRIPTOR& OutputVoices::getDesc(unsigned index) const {
    assert(index < mDescs.size());
    return mDescs[index];
}

size_t OutputVoices::size() const {
    return mDescs.size();
}

void OutputVoices::apply() {
    if (mDescs.size() == 0) {
        Debug::logWarning("Descriptors is empty.");
        return;
    }

    XAUDIO2_VOICE_SENDS sends = { 0 };
    sends.SendCount = mDescs.size();
    sends.pSends = mDescs.data();
    auto res = mVoice.getXAudio2Voice()->SetOutputVoices(&sends);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set output voices.");
    }
#endif // _DEBUG
}
