#pragma once

#include "../Voice/IVoice.h"
#include <memory>

class SoundFade;
class SoundPan;

//ボイスの音量を扱うクラス
class SoundVolume {
public:
    SoundVolume(IVoice& voice, unsigned inChannels, unsigned outChannels);
    ~SoundVolume();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// ボイスの音量を変更する
    /// </summary>
    /// <param name="volume">設定したい音量</param>
    void setVolume(float volume);

    /// <summary>
    /// デシベルで音量を変更する
    /// </summary>
    /// <param name="decibels">設定したいデシベル値</param>
    void setVolumeByDecibels(float decibels);

    /// <summary>
    /// ボリューム値からデシベル値に変換する
    /// </summary>
    /// <param name="volume">変換させたいボリューム値</param>
    /// <returns>変換されたデシベル値</returns>
    static float amplitudeRatioToDecibels(float volume);

    /// <summary>
    /// デシベル値からボリューム値に変換する
    /// </summary>
    /// <param name="decibels">変換させたいデシベル値</param>
    /// <returns>変換されたボリューム値</returns>
    static float decibelsToAmplitudeRatio(float decibels);

    /// <summary>
    /// 現在設定されている音量を返す
    /// </summary>
    /// <returns>現在の音量</returns>
    float getVolume() const;

    /// <summary>
    /// フェード専門クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundFade& getFade() const;

    /// <summary>
    /// パンニング専門クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundPan& getPan() const;

private:
    SoundVolume(const SoundVolume&) = delete;
    SoundVolume& operator=(const SoundVolume&) = delete;

private:
    IVoice& mVoice;
    std::unique_ptr<SoundFade> mFader;
    std::unique_ptr<SoundPan> mSoundPan;
    float mCurrentVolume;
};
