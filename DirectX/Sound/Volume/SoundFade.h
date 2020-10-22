#pragma once

#include <functional>

class SoundVolume;

//サウンドフェード専門クラス
class SoundFade {
public:
    SoundFade(SoundVolume& soundVolume);
    ~SoundFade();

    /// <summary>
    /// フェード指定
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    /// <param name="f">フェード終了時に設定されている関数を実行する</param>
    void settings(float targetVolume, float targetTime, const std::function<void()>& f = nullptr);

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// フェードしている途中か
    /// </summary>
    /// <returns></returns>
    bool isFading() const;

private:
    SoundFade(const SoundFade&) = delete;
    SoundFade& operator=(const SoundFade&) = delete;

    //フェードを進める
    void updateFade();
    //lerpの時間を進める
    void updateLerpTimer();
    //音量をフェードアップデートする
    void nextVolumeFading();
    //フェードの終わりに
    void fadeEnd();

private:
    SoundVolume& mSoundVolume;
    std::function<void()> mFadeEndFunc;
    float mTargetVolume;
    float mTargetTime;
    float mBeforeVolume;
    float mTimeRate;
    bool mIsFading;
};
