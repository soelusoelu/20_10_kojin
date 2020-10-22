#pragma once

#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

class __declspec(uuid("{101E38E7-407D-48E7-863F-E9936B187D2C}"))
TestEffectParameters : public CXAPOParametersBase {
public:
    TestEffectParameters();
    ~TestEffectParameters();

    //�ŏ��̈�񂾂��Ă΂��A�l��ێ����邽�߂̊֐�
    STDMETHOD(LockForProcess)(
        UINT32 InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
        UINT32 OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters
        );

    //���b100�񏈗�����郁�C���֐�
    //���ۂɃG�t�F�N�g���|����Ƃ�
    STDMETHOD_(void, Process)(
        UINT32 InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
        UINT32 OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
        BOOL IsEnabled
        );

    //voice->setEffectParameters�̃p�����[�^�����`�F�b�N�Ƃ��Ɏg��
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

    STDMETHOD_(void, GetParameters)(
        void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    //�v���p�e�B
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(TestEffectParameters),
        L"TestEffectParameters",
        L"Copyright",
        1, 0,
        XAPOBASE_DEFAULT_FLAG,
        1, 1, 1, 1
    };

    //���̓t�H�[�}�b�g�̋L���ꏊ
    WAVEFORMATEX inputFmt_;
    //�o�̓t�H�[�}�b�g�̋L���ꏊ
    WAVEFORMATEX outputFmt_;
    //�p�����[�^�̎�M�ꏊ
    float mParams[3];
};
