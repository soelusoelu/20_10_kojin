#include "Sound3DCalculator.h"
#include "Emitter/Sound3DEmitter.h"
#include "Emitter/DspSetter.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

//事前チェック
static_assert(sizeof(Vector3) == sizeof(X3DAUDIO_VECTOR), "Vector3 size does not match.");
static_assert(sizeof(Sound3DListenerParam) == sizeof(X3DAUDIO_LISTENER), "Listener sizes do not match.");
static_assert(sizeof(Sound3DEmitterParam) == sizeof(X3DAUDIO_EMITTER), "Emitter sizes do not match.");

Sound3DCalculator::Sound3DCalculator(const MasteringVoice& masteringVoice) :
    mInstance(),
    mInitialized(false) {
    auto res = X3DAudioInitialize(masteringVoice.getChannelMask(), X3DAUDIO_SPEED_OF_SOUND, mInstance);
    if (FAILED(res)) {
        Debug::logError("Failed 3d sound initialized.");
        return;
    }

    mInitialized = true;
}

Sound3DCalculator::~Sound3DCalculator() = default;

void Sound3DCalculator::calculate(const Sound3DListenerParam& listener, const Sound3DEmitter& emitter) const {
    if (!mInitialized) {
        return;
    }

    X3DAUDIO_LISTENER l;
    memcpy(&l, &listener, sizeof(Sound3DListenerParam));
    X3DAUDIO_EMITTER e;
    memcpy(&e, &emitter.getEmitter(), sizeof(Sound3DEmitterParam));
    //必要に応じたフラグを設定する
    auto flags = getFlagsFromEmitter(emitter);
    //本計算
    X3DAudioCalculate(mInstance, &l, &e, flags.get(), &emitter.getDspSetter().getDspSetting());

    //計算結果をソースボイスに適用する
    emitter.getDspSetter().applyToSourceVoice(flags);
}

Flag Sound3DCalculator::getFlagsFromEmitter(const Sound3DEmitter& emitter) const {
    //フラグを初期化する
    Flag flags;

    //行列計算は絶対
    flags.set(X3DAUDIO_CALCULATE_MATRIX);
    //ローパスフィルタ
    if (emitter.isCalculateLPFDirect()) {
        flags.set(X3DAUDIO_CALCULATE_LPF_DIRECT);
    }
    //リバーブ
    if (emitter.isCalculateReverb()) {
        flags.set(X3DAUDIO_CALCULATE_REVERB);
        flags.set(X3DAUDIO_CALCULATE_LPF_REVERB);
    }
    //ドップラー効果
    if (emitter.isCalculateDoppler()) {
        flags.set(X3DAUDIO_CALCULATE_DOPPLER);
    }

    return flags;
}
