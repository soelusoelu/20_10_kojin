#pragma once

#include "SourceVoiceInitParam.h"
#include "../IVoice.h"
#include "../../Data/SoundData.h"
#include "../../Loader/ISoundLoader.h"
#include "../../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class MasteringVoice;
class SoundPlayer;
class Sound3DEmitter;

//IXAudio2SourceVoiceラッパークラス
class SourceVoice : public IVoice {
public:
    SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param);
    ~SourceVoice();

    /// <summary>
    /// XAudio2ボイスインターフェースを返す
    /// </summary>
    /// <returns></returns>
    virtual IXAudio2Voice* getXAudio2Voice() const override;

    /// <summary>
    /// ボイスの詳細を返す
    /// </summary>
    /// <returns></returns>
    virtual const VoiceDetails& getVoiceDetails() const override;

    /// <summary>
    /// サウンド音量設定クラスを返す
    /// </summary>
    /// <returns></returns>
    virtual SoundVolume& getSoundVolume() const override;

    /// <summary>
    /// ボイスの出力先管理クラスを返す
    /// </summary>
    /// <returns></returns>
    virtual OutputVoices& getOutputVoices() const override;

    /// <summary>
    /// サウンドエフェクト専門クラスを返す
    /// </summary>
    /// <returns></returns>
    virtual SoundEffect& getSoundEffect() const override;

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// IXAudio2SourceVoiceを返す
    /// </summary>
    /// <returns></returns>
    IXAudio2SourceVoice* getXAudio2SourceVoice() const;

    /// <summary>
    /// サウンドデータを返す
    /// </summary>
    /// <returns></returns>
    SoundData& getSoundData() const;

    /// <summary>
    /// サウンド再生クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundPlayer& getSoundPlayer() const;

    /// <summary>
    /// エミッターを返す
    /// </summary>
    /// <returns></returns>
    Sound3DEmitter& getEmitter() const;

    /// <summary>
    /// 3D演算をするか
    /// </summary>
    /// <returns></returns>
    bool isCalculate3D() const;

private:
    SourceVoice(const SourceVoice&) = delete;
    SourceVoice& operator=(const SourceVoice&) = delete;

private:
    IXAudio2SourceVoice* mXAudio2SourceVoice;
    VoiceDetails mDetails;
    std::unique_ptr<SoundData> mSoundData;
    std::unique_ptr<SoundVolume> mSoundVolume;
    std::unique_ptr<OutputVoices> mOutputVoices;
    std::unique_ptr<SoundEffect> mSoundEffect;
    std::unique_ptr<SoundPlayer> mSoundPlayer;
    std::unique_ptr<Sound3DEmitter> mEmitter;
};
