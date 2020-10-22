#pragma once

#include "../SoundEffectUtility.h"
#include <xapobase.h>
#include <vector>

#pragma comment(lib, "xapobase.lib")

namespace MyFilter {
//単極ハイパスフィルタ
class __declspec(uuid("{BC07FB1C-5822-4013-A37B-7183C53570B4}"))
    HighPassOnePoleFilter : public CXAPOParametersBase {
public:
    HighPassOnePoleFilter();
    ~HighPassOnePoleFilter();

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
    HighPassOnePoleFilter(const HighPassOnePoleFilter&) = delete;
    HighPassOnePoleFilter& operator=(const HighPassOnePoleFilter&) = delete;

    void highPassOnePoleFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam);

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(HighPassOnePoleFilter),
        L"HighPassOnePoleFilter",
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
    //カットオフ周波数
    float mFrequency[EFFECT_PARAMETER_SIZE];

    //出力チャンネル配列
    std::vector<float*> mOutCh;
    //フィルタ係数
    //a: 元の音を残すブレンド率(%)
    //b: 新しい音のブレンド率(%)
    float mA1;
    float mB0;
    //最後の演算ボリューム値
    float mLastVolume;
};
}
