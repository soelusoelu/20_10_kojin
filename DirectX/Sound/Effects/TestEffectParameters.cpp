#include "TestEffectParameters.h"
#include "../../DebugLayer/Debug.h"

TestEffectParameters::TestEffectParameters() :
    CXAPOParametersBase(&xapoRegProp_, (BYTE*)mParams, sizeof(float), FALSE),
    inputFmt_(),
    outputFmt_() {
    ZeroMemory(mParams, sizeof(mParams));
}

TestEffectParameters::~TestEffectParameters() = default;

STDMETHODIMP_(HRESULT __stdcall) TestEffectParameters::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    inputFmt_ = *pInputLockedParameters[0].pFormat;
    outputFmt_ = *pOutputLockedParameters[0].pFormat;

    return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) TestEffectParameters::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inputParam = pInputProcessParameters[0];
    auto& outputParam = pOutputProcessParameters[0];

    if (IsEnabled) { //有効
        //3配列のうち有効なアドレスの取得
        float* param = reinterpret_cast<float*>(BeginProcess());

        memcpy(outputParam.pBuffer, inputParam.pBuffer, outputFmt_.nBlockAlign * inputParam.ValidFrameCount);

        outputParam.ValidFrameCount = inputParam.ValidFrameCount;
        outputParam.BufferFlags = inputParam.BufferFlags;

        EndProcess();
    } else { //無効
        if (inputParam.pBuffer != outputParam.pBuffer) {
            memcpy(outputParam.pBuffer, inputParam.pBuffer, outputFmt_.nBlockAlign * inputParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) TestEffectParameters::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    //サイズが一致していたらOK
    if (ParameterByteSize == sizeof(float)) {
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong XAPO parameter size");
    }
}

STDMETHODIMP_(void __stdcall) TestEffectParameters::GetParameters(void* pParameters, UINT32 ParameterByteSize) {
    *reinterpret_cast<float*>(pParameters) = 1.f;
}
