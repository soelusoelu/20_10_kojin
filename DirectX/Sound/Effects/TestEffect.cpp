#include "TestEffect.h"

TestEffect::TestEffect() :
    CXAPOBase(&xapoRegProp_),
    inputFmt_(),
    outputFmt_() {
}

TestEffect::~TestEffect() = default;

STDMETHODIMP_(HRESULT __stdcall) TestEffect::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    inputFmt_ = *pInputLockedParameters[0].pFormat;
    outputFmt_ = *pOutputLockedParameters[0].pFormat;

    return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) TestEffect::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inputParam = pInputProcessParameters[0];
    auto& outputParam = pOutputProcessParameters[0];

    if (IsEnabled) { //—LŒø
        memcpy(outputParam.pBuffer, inputParam.pBuffer, outputFmt_.nBlockAlign * inputParam.ValidFrameCount);

        outputParam.ValidFrameCount = inputParam.ValidFrameCount;
        outputParam.BufferFlags = inputParam.BufferFlags;
    } else { //–³Œø
        if (inputParam.pBuffer != outputParam.pBuffer) {
            memcpy(outputParam.pBuffer, inputParam.pBuffer, outputFmt_.nBlockAlign * inputParam.ValidFrameCount);
        }
    }
}
