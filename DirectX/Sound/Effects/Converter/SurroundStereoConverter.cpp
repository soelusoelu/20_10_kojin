#include "SurroundStereoConverter.h"
#include "../../../DebugLayer/Debug.h"

SurroundStereoConverter::SurroundStereoConverter() :
    CXAPOBase(&xapoRegProp_),
    mInputFmt(),
    mOutputFmt() {
}

SurroundStereoConverter::~SurroundStereoConverter() = default;

STDMETHODIMP_(HRESULT __stdcall) SurroundStereoConverter::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) SurroundStereoConverter::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];
    //バッファの取得
    float* inBuf = static_cast<float*>(inParam.pBuffer);
    float* outBuf = static_cast<float*>(outParam.pBuffer);

    //出力先初期位置をセット
    float* left = outBuf;
    float* right = outBuf + 1;

    //サンプル数分まわす
    for (size_t i = 0; i < inParam.ValidFrameCount; i++) {
        *left = 0.f;
        *right = 0.f;

        //LRにチャンネル数分廻して合成する
        for (size_t ch = 0; ch < mInputFmt.nChannels; ch++) {
            *left += *inBuf * L_MIX[ch];
            *right += *inBuf * R_MIX[ch];
            ++inBuf;
        }
        left += 2;
        right += 2;
    }
}

STDMETHODIMP_(void __stdcall) SurroundStereoConverter::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    Debug::logWarning("It is not an effect that can set parameters.");
}

STDMETHODIMP_(void __stdcall) SurroundStereoConverter::GetParameters(void* pParameters, UINT32 ParameterByteSize) {
    Debug::logWarning("It is not an effect that can get parameters.");
}
