#pragma once

#include <xaudio2.h>

class SourceVoice;
class SoundPlayer;

//周波数を扱うクラス
class Frequency {
public:
    Frequency(SourceVoice& sourceVoice, SoundPlayer& player, float maxFrequencyRatio);
    ~Frequency();

    /// <summary>
    /// ボイスの再生ピッチを変える
    /// </summary>
    /// <param name="pitch">再生速度 ソースボイス作成時のピッチ上限を超える値はクリッピングされる</param>
    void setFrequencyRatio(float pitch);

    /// <summary>
    /// 現在の再生ピッチを返す
    /// </summary>
    /// <returns></returns>
    float getFrequencyRatio() const;

private:
    Frequency(const Frequency&) = delete;
    Frequency& operator=(const Frequency&) = delete;

private:
    SourceVoice& mSourceVoice;
    SoundPlayer& mPlayer;
    float mCurrentFrequencyRatio;
    float mMaxFrequencyRatio;
};
