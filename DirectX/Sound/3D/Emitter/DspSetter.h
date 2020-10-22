#pragma once

#include "../../Loader/WaveFormat.h"
#include "../../../Device/Flag.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class SubmixVoice;

//3Dサウンドを反映するクラス
class DspSetter {
public:
    DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const SubmixVoice& reverb, const WaveFormat& format);
    ~DspSetter();

    /// <summary>
    /// ソースボイスに3D計算した結果を適用する
    /// </summary>
    void applyToSourceVoice(const Flag& flag);

    /// <summary>
    /// dspを返す
    /// </summary>
    /// <returns></returns>
    X3DAUDIO_DSP_SETTINGS& getDspSetting() const;

private:
    DspSetter(const DspSetter&) = delete;
    DspSetter& operator=(const DspSetter&) = delete;

private:
    SourceVoice& mSourceVoice;
    const MasteringVoice& mMasteringVoice;
    //リバーブ用サブミックスボイス
    const SubmixVoice& mReverb;
    //計算結果
    std::unique_ptr<X3DAUDIO_DSP_SETTINGS> mDspSettings;
    //行列係数
    std::vector<float> mMainVolumes;
    std::vector<float> mReverbVolumes;
};
