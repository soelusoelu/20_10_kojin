#pragma once

#include "../SoundEffectUtility.h"
#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

class __declspec(uuid("{A361BBD1-67D0-4BA6-8772-9D2E868CF28E}"))
VolumeMeter : public CXAPOParametersBase {
public:
    VolumeMeter();
    ~VolumeMeter();

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

    //もしアクセスしたとしても落とさないために
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

    STDMETHOD_(void, GetParameters)(
        void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    VolumeMeter(const VolumeMeter&) = delete;
    VolumeMeter& operator=(const VolumeMeter&) = delete;

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(VolumeMeter),
        L"VolumeMeter",
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
    float mPeakLevels[EFFECT_PARAMETER_SIZE];

    //最大ボリューム値
    float mMaxVolume;
};
