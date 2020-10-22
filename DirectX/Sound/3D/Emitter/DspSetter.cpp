#include "DspSetter.h"
#include "../../Player/Frequency.h"
#include "../../Player/SoundPlayer.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Voice/SourceVoice/SourceVoice.h"
#include "../../Voice/SubmixVoice/SubmixVoice.h"
#include "../../Volume/SoundPan.h"
#include "../../Volume/SoundVolume.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"
#include <algorithm>

DspSetter::DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const SubmixVoice& reverb, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mMasteringVoice(masteringVoice),
    mReverb(reverb),
    mDspSettings(std::make_unique<X3DAUDIO_DSP_SETTINGS>()) {
    //マスターボイスを出力先に指定
    mSourceVoice.getOutputVoices().addOutputVoice(masteringVoice, true);

    const auto inCh = format.channels;
    const auto outCh = masteringVoice.getVoiceDetails().channels;

    //DSPに必要な値を設定
    mDspSettings->SrcChannelCount = inCh;
    mDspSettings->DstChannelCount = outCh;
    mMainVolumes.resize(inCh * outCh);
    mDspSettings->pMatrixCoefficients = mMainVolumes.data();
    mDspSettings->pDelayTimes = nullptr;

    //リバーブボリューム
    mReverbVolumes.resize(outCh);
}

DspSetter::~DspSetter() = default;

void DspSetter::applyToSourceVoice(const Flag& flag) {
    //3D計算(絶対設定されているはず)
    if (flag.check(X3DAUDIO_CALCULATE_MATRIX)) {
        mSourceVoice.getSoundVolume().getPan().panOutputVoice(mMasteringVoice, mMainVolumes.data());
    } else {
        Debug::logError("X3DAUDIO_CALCULATE_MATRIX is not flagged.");
    }
    //ローパスフィルタ直接パス
    if (flag.check(X3DAUDIO_CALCULATE_LPF_DIRECT)) {
        XAUDIO2_FILTER_PARAMETERS param = { XAUDIO2_FILTER_TYPE::LowPassFilter, 2.f * sinf(Math::PI / 6.f * mDspSettings->LPFDirectCoefficient), 1.414f };
        mSourceVoice.getXAudio2Voice()->SetOutputFilterParameters(mMasteringVoice.getXAudio2Voice(), &param);
        //mSourceVoice.getSoundEffect().getEffectCollection().getFilter().lowPassFilter(mDspSettings->LPFDirectCoefficient * mSourceVoice.getVoiceDetails().sampleRate / 6.f);
    }
    //リバーブ
    if (flag.check(X3DAUDIO_CALCULATE_REVERB)) {
        std::fill(mReverbVolumes.begin(), mReverbVolumes.end(), mDspSettings->ReverbLevel);
        mSourceVoice.getSoundVolume().getPan().panOutputVoice(mReverb, mReverbVolumes.data());
    }
    //ローパスフィルタリバーブパス
    if (flag.check(X3DAUDIO_CALCULATE_LPF_REVERB)) {
        XAUDIO2_FILTER_PARAMETERS param = { XAUDIO2_FILTER_TYPE::LowPassFilter, 2.f * sinf(Math::PI / 6.f * mDspSettings->LPFReverbCoefficient), 1.414f };
        mSourceVoice.getXAudio2Voice()->SetOutputFilterParameters(mReverb.getXAudio2Voice(), &param);
        //mReverb->getSoundEffect().getEffectCollection().getFilter().lowPassFilter(mDspSettings->LPFReverbCoefficient * mReverb->getVoiceDetails().sampleRate / 6.f);
    }
    //ドップラー
    if (flag.check(X3DAUDIO_CALCULATE_DOPPLER)) {
        mSourceVoice.getSoundPlayer().getFrequency().setFrequencyRatio(mDspSettings->DopplerFactor);
    }
}

X3DAUDIO_DSP_SETTINGS& DspSetter::getDspSetting() const {
    return *mDspSettings;
}
