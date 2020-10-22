#include "Sound3DEmitter.h"
#include "DspSetter.h"
#include "../../Effects/SoundEffect.h"
#include "../../Effects/SoundEffectCollection.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Voice/SourceVoice/SourceVoice.h"
#include "../../Voice/SubmixVoice/SubmixVoice.h"
#include "../../Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include "../../XAudio2/SoundEngine.h"
#include "../../../Device/Time.h"

Sound3DEmitter::Sound3DEmitter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    mEmitter(),
    mReverb(nullptr),
    mReverbID(0),
    mPreviousPos(Vector3::zero),
    mIsCalculateLPFDirect(true),
    mDspSetter(nullptr) {
    //リバーブサブミックスボイス作成
    createReverbSubmixVoice(masteringVoice);
    //リバーブサブミックスボイスを出力先に指定
    sourceVoice.getOutputVoices().addOutputVoice(*mReverb, true);
    //DSP作成
    mDspSetter = std::make_unique<DspSetter>(sourceVoice, masteringVoice, *mReverb, format);
    //エミッター初期化
    initializeEmitter(format);
}

Sound3DEmitter::~Sound3DEmitter() = default;

void Sound3DEmitter::update() {
    mEmitter.velocity = (mEmitter.position - mPreviousPos) / Time::deltaTime;
    mPreviousPos = mEmitter.position;
}

void Sound3DEmitter::setPosition(const Vector3& pos) {
    mEmitter.position = pos;
}

void Sound3DEmitter::setDopplerScale(float scale) {
    scale = Math::clamp<float>(scale, 0.f, Math::infinity);
    mEmitter.dopplerScaler = scale;
}

bool Sound3DEmitter::isCalculateDoppler() const {
    return !Math::nearZero(mEmitter.dopplerScaler);
}

void Sound3DEmitter::setCalculateLPFDirect(bool value) {
    mIsCalculateLPFDirect = value;
}

bool Sound3DEmitter::isCalculateLPFDirect() const {
    return mIsCalculateLPFDirect;
}

void Sound3DEmitter::setCalculateReverb(bool value) {
    auto& effect = mReverb->getSoundEffect();
    (value) ? effect.enable(mReverbID) : effect.disable(mReverbID);
}

bool Sound3DEmitter::isCalculateReverb() const {
    return mReverb->getSoundEffect().getEnabled(mReverbID);
}

const Sound3DEmitterParam& Sound3DEmitter::getEmitter() const {
    return mEmitter;
}

DspSetter& Sound3DEmitter::getDspSetter() const {
    return *mDspSetter;
}

void Sound3DEmitter::createReverbSubmixVoice(const MasteringVoice& masteringVoice) {
    SubmixVoiceInitParam param;
    //リバーブのチャンネル数は1
    param.channels = 1;
    //マスターボイスにサンプル数を合わせる
    param.inputSampleRate = masteringVoice.getVoiceDetails().sampleRate;
    //リバーブ作成
    mReverb = SoundEngine::instance().createSubmixVoice(param);
    //mReverbID = mReverb->getSoundEffect().getEffectCollection().reverb();
    mReverbID = mReverb->getSoundEffect().getEffectCollection().simpleReverb();
}

void Sound3DEmitter::initializeEmitter(const WaveFormat& format) {
    const auto inCh = format.channels;

    mEmitter.cone = &EMITTER_CONE;
    mEmitter.channelCount = inCh;
    mEmitter.channelRadius = 1.f;
    mAzimuths.resize(inCh);
    mEmitter.channelAzimuths = mAzimuths.data();
    mEmitter.innerRadius = 2.f;
    mEmitter.innerRadiusAngle = Math::PI / 4.f;
    mEmitter.volumeCurve = &DEFAULT_LINEAR_CURVE;
    mEmitter.lfeCurve = &EMITTER_LFE_CURVE;
    mEmitter.lpfDirectCurve = nullptr; //デフォルトカーブを使用
    mEmitter.lpfReverbCurve = nullptr; //デフォルトカーブを使用
    mEmitter.reverbCurve = &EMITTER_REVERB_CURVE;
    mEmitter.curveDistanceScaler = 14.f;
    mEmitter.dopplerScaler = 0.f;
}
