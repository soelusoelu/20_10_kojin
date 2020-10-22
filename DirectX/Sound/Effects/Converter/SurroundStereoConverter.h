#pragma once

#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

//サラウンドからステレオへの変換エフェクト
class __declspec(uuid("{10B7277B-DB62-4CE2-803B-C1F04E178B7F}"))
    SurroundStereoConverter : public CXAPOBase {
public:
    SurroundStereoConverter();
    ~SurroundStereoConverter();

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
    SurroundStereoConverter(const SurroundStereoConverter&) = delete;
    SurroundStereoConverter& operator=(const SurroundStereoConverter&) = delete;

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(SurroundStereoConverter),
        L"SurroundStereoConverter",
        L"Copyright",
        1, 0,
        XAPOBASE_DEFAULT_FLAG,
        1, 1, 1, 1
    };

    //入力フォーマットの記憶場所
    WAVEFORMATEX mInputFmt;
    //出力フォーマットの記憶場所
    WAVEFORMATEX mOutputFmt;

    //6ch -> 2ch 変換テーブル
    static constexpr float L_MIX[6] = { 0.9f, 0.1f, 0.6f, 0.4f, 0.9f, 0.0f };
    static constexpr float R_MIX[6] = { 0.1f, 0.9f, 0.6f, 0.4f, 0.0f, 0.9f };
};
