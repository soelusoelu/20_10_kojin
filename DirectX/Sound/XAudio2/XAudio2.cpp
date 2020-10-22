#include "XAudio2.h"
#include "../Loader/WaveFormat.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Voice/SubmixVoice/SubmixVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Device/Flag.h"
#include <cassert>

XAudio2::XAudio2(bool* successFlag) :
    mXAudio2(nullptr) {
    //インスタンスを1つに制限
    assert(!mInstantiated);
    mInstantiated = true;

    *successFlag = createXAudio2();
}

XAudio2::~XAudio2() {
    mInstantiated = false;
}

MasteringVoice* XAudio2::createMasteringVoice() const {
    IXAudio2MasteringVoice* masteringVoice = nullptr;
    auto res = mXAudio2->CreateMasteringVoice(&masteringVoice);

    if (FAILED(res)) {
        Debug::logError("Failed created mastering voice.");
        return nullptr;
    }

    return new MasteringVoice(masteringVoice);
}

std::unique_ptr<SourceVoice> XAudio2::createSourceVoice(MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WAVEFORMATEX& format, const SourceVoiceInitParam& param) const {
    IXAudio2SourceVoice* sourceVoice = nullptr;
    auto res = mXAudio2->CreateSourceVoice(&sourceVoice, &format, param.flags.get(), param.maxFrequencyRatio, param.callback, param.sendList, param.effectChain);

    if (FAILED(res)) {
        Debug::logError("Failed created source voice.");
        return nullptr;
    }

    WaveFormat fmt(format);
    return std::make_unique<SourceVoice>(sourceVoice, masteringVoice, loader, fmt, param);
}

std::unique_ptr<SubmixVoice> XAudio2::createSubmixVoice(MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param) const {
    IXAudio2SubmixVoice* submixVoice = nullptr;
    auto res = mXAudio2->CreateSubmixVoice(&submixVoice, param.channels, param.inputSampleRate, param.flags.get(), param.processingStage, param.sendList, param.effectChain);

    if (FAILED(res)) {
        Debug::logError("Failed created submix voice.");
        return nullptr;
    }

    return std::make_unique<SubmixVoice>(submixVoice, masteringVoice, param);
}

bool XAudio2::createXAudio2() {
    auto res = XAudio2Create(&mXAudio2, 0);
    if (FAILED(res)) {
        Debug::logError("Failed created XAudio2.");
        return false;
    }

    //デバッグ指定
#ifdef _DEBUG
    XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
    debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
    debug.BreakMask = XAUDIO2_LOG_ERRORS;
    mXAudio2->SetDebugConfiguration(&debug, 0);
#endif // _DEBUG

    return true;
}
