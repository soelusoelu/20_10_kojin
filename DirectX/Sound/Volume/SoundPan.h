#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>

//音の定位を扱うクラス
class SoundPan {
public:
    SoundPan(IVoice& voice, unsigned inChannels, unsigned outChannels);
    ~SoundPan();

    /// <summary>
    /// 音量配列から音をパンする
    /// </summary>
    /// <param name="volumes">音量配列</param>
    void pan(const float volumes[]);

    /// <summary>
    /// 指定したボイスをパンする
    /// </summary>
    /// <param name="voice">パンするボイス</param>
    /// <param name="volumes">音量配列</param>
    void panOutputVoice(const IVoice& voice, const float volumes[]);

    /// <summary>
    /// 位置によって音をパンする
    /// </summary>
    /// <param name="positionX">音を鳴らす位置(0～1920)</param>
    void panFromPositionX(float positionX);

    /// <summary>
    /// 音の定位を中心に戻す
    /// </summary>
    void panCenter();

private:
    SoundPan(const SoundPan&) = delete;
    SoundPan& operator=(const SoundPan&) = delete;

    //出力先を決定する
    void selectOutput(const float volumes[]);
    //パンを実行する
    void setOutputMatrix(IXAudio2Voice* outputVoice, unsigned outChannels, const float volumes[]);

private:
    IVoice& mVoice;
    static constexpr float CENTER_VOLUME = 0.707f;
    const unsigned INPUT_CHANNELS;
    const unsigned MASTER_VOICE_CHANNELS;
};
