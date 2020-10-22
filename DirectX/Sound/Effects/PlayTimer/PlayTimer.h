#pragma once

#include "PlayTimerParam.h"
#include "../SoundEffectUtility.h"
#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

//再生時間設定/取得エフェクト
class __declspec(uuid("{16A1BD38-43A6-4822-8019-D5FA940215BB}"))
    PlayTimer : public CXAPOParametersBase {
public:
    PlayTimer();
    ~PlayTimer();

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

    //再生時間を設定する(単位: 秒)
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

    //現在の再生時間を返す(単位: 秒)
    STDMETHOD_(void, GetParameters)(
        void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    PlayTimer(const PlayTimer&) = delete;
    PlayTimer& operator= (const PlayTimer&) = delete;

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(PlayTimer),
        L"PlayTimer",
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
    PlayTimerParam mParams[EFFECT_PARAMETER_SIZE];

    //現在の再生時間(単位: バイト)
    unsigned mCurrentTime;
};
