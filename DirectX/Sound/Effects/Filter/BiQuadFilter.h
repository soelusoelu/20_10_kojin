#pragma once

#include "FilterParam.h"
#include "FilterType.h"
#include "../SoundEffectUtility.h"
#include <xapobase.h>
#include <vector>

#pragma comment(lib, "xapobase.lib")

//バイクアッドフィルタ
class __declspec(uuid("{5EA86338-E8D7-4666-A086-A195A0F61886}"))
    BiQuadFilter : public CXAPOParametersBase {
public:
    BiQuadFilter(FilterType type);
    ~BiQuadFilter();

    //最初の一回だけ呼ばれる、値を保持するための関数
    STDMETHOD(LockForProcess)(
        UINT32 InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
        UINT32 OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters
        );

    //毎秒100回処理されるメイン関数
    //実際にエフェクトを掛けるとこ
    STDMETHOD_(void, Process)(
        UINT32 InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
        UINT32 OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
        BOOL IsEnabled
        );

    //パラメータ設定関数
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    BiQuadFilter(const BiQuadFilter&) = delete;
    BiQuadFilter& operator=(const BiQuadFilter&) = delete;

    void computeFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam);

    //入力信号から出力信号を計算する
    float computeVolume(float inVolume);
    //フィルタ係数を計算する
    void computeCoefficient(float cutoffFrequency, float qualityFactor, unsigned sampleRate);

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(BiQuadFilter),
        L"BiQuadFilter",
        L"Copyright",
        1, 0,
        XAPOBASE_DEFAULT_FLAG,
        1, 1, 1, 1
    };

    //入力フォーマットの記憶場所
    WAVEFORMATEX mInputFmt;
    //出力フォーマットの記憶場所
    WAVEFORMATEX mOutputFmt;
    //パラメータの受信場所
    FilterParam mParam[EFFECT_PARAMETER_SIZE];

    //フィルタの種類
    FilterType mType;
    //出力チャンネル配列
    std::vector<float*> mOutCh;

    //2つ前までの入力信号
    float mOldIn[2];
    //2つ前までの演算結果
    float mOldOut[2];
    //フィルタ係数
    float mA[3];
    float mB[3];
};
