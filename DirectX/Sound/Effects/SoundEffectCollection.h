#pragma once

#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "../Voice/IVoice.h"
#include <memory>

class SoundFilter;

//すべてのエフェクトにアクセスできるクラス
class SoundEffectCollection {
public:
    SoundEffectCollection(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter);
    ~SoundEffectCollection();

    /// <summary>
    /// リバーブ効果(残響)を掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int reverb();

    /// <summary>
    /// 簡易版リバーブを掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int simpleReverb();

    /// <summary>
    /// エコーを掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int echo();

    /// <summary>
    /// イコライザー
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int equalizer();

    /// <summary>
    /// ボリュームメーター
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int volumeMeter();

    /// <summary>
    /// 再生時間設定/取得
    /// </summary>
    void getPlayTimer();

    /// <summary>
    /// フーリエ変換
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int discreteFourierTransform();

    /// <summary>
    /// フィルター管理クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundFilter& getFilter() const;

private:
    SoundEffectCollection(const SoundEffectCollection&) = delete;
    SoundEffectCollection& operator=(const SoundEffectCollection&) = delete;

public:
    static constexpr int PLAY_TIMER_ID = 0;

private:
    IEffectCreater& mEffectCreater;
    std::unique_ptr<SoundFilter> mFilter;
};
