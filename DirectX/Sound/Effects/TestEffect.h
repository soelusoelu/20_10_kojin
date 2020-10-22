#pragma once

#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

class __declspec(uuid("{A3FD60BA-9070-48A0-ACE1-3A357DF8035D}"))
TestEffect : public CXAPOBase {
public:
    TestEffect();
    ~TestEffect();

    STDMETHOD(LockForProcess)(
        UINT32 InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
        UINT32 OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters
    );

    STDMETHOD_(void, Process)(
        UINT32 InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
        UINT32 OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
        BOOL IsEnabled
    );

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(TestEffect),
        L"TestEffect",
        L"Copyright",
        1, 0,
        XAPOBASE_DEFAULT_FLAG,
        1, 1, 1, 1
    };

    //入力フォーマットの記憶場所
    WAVEFORMATEX inputFmt_;
    //出力フォーマットの記憶場所
    WAVEFORMATEX outputFmt_;
};
