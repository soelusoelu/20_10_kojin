#include "SoundEffect.h"
#include "SoundEffectCollection.h"
#include "../Voice/VoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>

SoundEffect::SoundEffect(IVoice& voice) :
    mVoice(voice),
    mEffectCollection(std::make_unique<SoundEffectCollection>(voice, *this, *this)),
    mDescs(),
    mIsApplied(false) {
}

SoundEffect::~SoundEffect() {
    for (auto&& desc : mDescs) {
        desc.pEffect->Release();
        desc.pEffect = nullptr;
    }
}

void SoundEffect::setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) {
    if (!canAccessEffect(effectID, parameters)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->SetEffectParameters(effectID, parameters, parametersByteSize);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}

void SoundEffect::getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const {
    if (!canAccessEffect(effectID, parameters)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->GetEffectParameters(effectID, parameters, parametersByteSize);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}

void SoundEffect::enable(int effectID) {
    if (!isValidID(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->EnableEffect(effectID);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed enable effect.");
    }
#endif // _DEBUG
}

void SoundEffect::disable(int effectID) {
    if (!isValidID(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->DisableEffect(effectID);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed disable effect.");
    }
#endif // _DEBUG
}

bool SoundEffect::getEnabled(int effectID) {
    if (!isValidID(effectID)) {
        return false;
    }

    BOOL enabled = 0;
    mVoice.getXAudio2Voice()->GetEffectState(effectID, &enabled);

    //enabledが1ならtrue
    return (enabled == 1);
}

SoundEffectCollection& SoundEffect::getEffectCollection() const {
    return *mEffectCollection;
}

int SoundEffect::createEffect(ISoundEffect* target, bool isApply) {
    XAUDIO2_EFFECT_DESCRIPTOR desc = { 0 };
    desc.InitialState = true;
    desc.OutputChannels = mVoice.getVoiceDetails().channels;
    bool res = target->create(&desc);
    if (!res) { //エフェクトの作成に失敗していたら-1
        return -1;
    }

    mDescs.emplace_back(desc);
    mIsApplied = false;

    if (isApply) {
        apply();
    }

    return mDescs.size() - 1;
}

int SoundEffect::createEffect(IUnknown* target, bool isApply) {
    if (!target) {
        return -1;
    }

    XAUDIO2_EFFECT_DESCRIPTOR desc = { 0 };
    desc.InitialState = true;
    desc.OutputChannels = mVoice.getVoiceDetails().channels;
    desc.pEffect = target;

    mDescs.emplace_back(desc);
    mIsApplied = false;

    if (isApply) {
        apply();
    }

    return mDescs.size() - 1;
}

void SoundEffect::apply() {
    if (mDescs.empty()) {
        Debug::logWarning("Effect descriptor is empty.");
        return;
    }

    XAUDIO2_EFFECT_CHAIN chain = { 0 };
    chain.EffectCount = mDescs.size();
    chain.pEffectDescriptors = mDescs.data();

    auto res = mVoice.getXAudio2Voice()->SetEffectChain(&chain);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect chain.");
    }
#endif // _DEBUG

    mIsApplied = true;
}

bool SoundEffect::isValidID(int effectID) const {
    //エフェクトを適用してあるか
    if (!mIsApplied) {
        Debug::logWarning("Effect not applied.");
        return false;
    }
    //IDが有効か
    if (effectID == -1) {
        Debug::logWarning("Invalid Effect ID.");
        return false;
    }
    //配列の範囲内か
    assert(effectID >= 0 && effectID < mDescs.size());

    return true;
}

bool SoundEffect::isValidParameters(const void* parameters) const {
    if (!parameters) {
        Debug::logWarning("Effect parameter is null");
        return false;
    }

    return true;
}

bool SoundEffect::canAccessEffect(int effectID, const void* parameters) const {
    if (!isValidID(effectID)) {
        return false;
    }
    if (!isValidParameters(parameters)) {
        return false;
    }

    return true;
}
