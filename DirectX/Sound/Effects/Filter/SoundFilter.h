#pragma once

#include "FilterType.h"
#include "../IEffectCreater.h"
#include "../IEffectParameter.h"
#include "../../Voice/IVoice.h"
#include <string>

//サウンドフィルター専門クラス
class SoundFilter {
public:
    SoundFilter(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter);
    ~SoundFilter();

    /// <summary>
    /// ローパスフィルター
    /// 指定した値より高い周波数をカットする
    /// </summary>
    /// <param name="cutoffFrequency">カットオフ周波数</param>
    /// <param name="qualityFactor">クオリティファクタ</param>
    void lowPassFilter(float cutoffFrequency, float qualityFactor = 0.707f);

    /// <summary>
    /// ローパスフィルター
    /// 指定した値より高い周波数をカットする
    /// </summary>
    /// <param name="radianFrequency">ラジアン周波数[0, 1]</param>
    /// <param name="qualityFactor">クオリティファクタ</param>
    void lowPassFilterFromRadianFrequency(float radianFrequency, float qualityFactor = 0.707f);

    /// <summary>
    /// 単極ローパスフィルター
    /// </summary>
    /// <param name="frequency">カットオフ周波数[0, 1]</param>
    void lowPassOnePoleFilter(float frequency);

    /// <summary>
    /// ハイパスフィルター
    /// 指定した値より低い周波数をカットする
    /// </summary>
    /// <param name="cutoffFrequency">カットオフ周波数</param>
    /// <param name="qualityFactor">クオリティファクタ</param>
    void highPassFilter(float cutoffFrequency, float qualityFactor = 0.707f);

    /// <summary>
    /// 単極ハイパスフィルター
    /// </summary>
    /// <param name="frequency">カットオフ周波数[0, 1]</param>
    void highPassOnePoleFilter(float frequency);

    /// <summary>
    /// バンドパスフィルタ
    /// 指定した値を中心に周りの周波数をカットする
    /// </summary>
    /// <param name="cutoffFrequency">中心周波数</param>
    /// <param name="qualityFactor">帯域幅</param>
    void bandPassFilter(float cutoffFrequency, float qualityFactor = 1.f);

    /// <summary>
    /// ノッチフィルタ
    /// 指定した値を中心に周波数をカットする
    /// </summary>
    /// <param name="cutoffFrequency">中心周波数</param>
    /// <param name="qualityFactor">帯域幅</param>
    void notchFilter(float cutoffFrequency, float qualityFactor = 1.f);

private:
    SoundFilter(const SoundFilter&) = delete;
    SoundFilter& operator=(const SoundFilter&) = delete;

    //フィルタの状態を更新
    void updateFilter(FilterType type, float cutoffFrequency, float qualityFactor);
    //フィルター作成
    void createFilter(FilterType type);
    //単極フィルター作成
    void createOnePoleFilter(FilterType type, float cutoffFrequency);
    //フィルタのパラメータを設定する
    void setFilterParameters(FilterType type, float cutoffFrequency, float qualityFactor);
    //カットオフ周波数を範囲内にクランプする
    float clampCutoffFrequency(float cutoffFrequency) const;
    //Qを範囲内にクランプする
    float clampQ(float qualityFactor) const;
    //カットオフ周波数からラジアン周波数に変換する
    float cutoffFrequencyToRadianFrequency(float cutoffFrequency) const;
    //ラジアン周波数からカットオフ周波数に変換する
    float radianFrequencyToCutoffFrequency(float radianFrequency) const;

private:
    IEffectCreater& mEffectCreater;
    IEffectParameter& mEffectParameter;
    IVoice& mVoice;
    int mFilterID;
};
