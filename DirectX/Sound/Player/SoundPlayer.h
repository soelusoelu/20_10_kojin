#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>

class SourceVoice;
class SoundStreaming;
class SoundPlayTimer;
class SoundLoop;
class Frequency;

//音の再生を扱うクラス
class SoundPlayer {
public:
    SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio);
    ~SoundPlayer();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// ストリーミング再生をする
    /// </summary>
    void playStreaming();

    /// <summary>
    /// フェードインしながら、ストリーミング再生をする
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void playStreamingFadeIn(float targetVolume, float targetTime);

    /// <summary>
    /// 再生開始地点を設定する
    /// </summary>
    /// <param name="point">再生開始地点(単位: 秒数)</param>
    void setPlayPoint(float point);

    /// <summary>
    /// 再生を一時停止する
    /// </summary>
    void pause();

    /// <summary>
    /// フェードアウトしながら再生を一時停止する
    /// </summary>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void pauseFadeOut(float targetTime);

    /// <summary>
    /// 再生を停止する
    /// </summary>
    void stop();

    /// <summary>
    /// フェードアウトしながら再生を停止する
    /// </summary>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void stopFadeOut(float targetTime);

    /// <summary>
    /// 再生が停止しているか
    /// </summary>
    /// <returns></returns>
    bool isStop() const;

    /// <summary>
    /// 再生時間専門クラスにアクセスする
    /// </summary>
    /// <returns></returns>
    SoundPlayTimer& getPlayTimer() const;

    /// <summary>
    /// ループ専門クラスにアクセスする
    /// </summary>
    /// <returns></returns>
    SoundLoop& getLoop() const;

    /// <summary>
    /// 周波数専門クラスにアクセスする
    /// </summary>
    /// <returns></returns>
    Frequency& getFrequency() const;

private:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    //ポーズ・ストップ共通処理
    bool pauseAndStop(unsigned flag);

private:
    SourceVoice& mSourceVoice;
    std::unique_ptr<SoundStreaming> mStreaming;
    std::unique_ptr<SoundPlayTimer> mPlayTimer;
    std::unique_ptr<SoundLoop> mLoop;
    std::unique_ptr<Frequency> mFrequency;
    bool mIsPlay;
};
