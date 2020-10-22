#include "PlayTimer.h"
#include "../../../DebugLayer/Debug.h"

PlayTimer::PlayTimer() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mParams), sizeof(PlayTimerParam), FALSE),
    mInputFmt(),
    mOutputFmt(),
    mCurrentTime(0) {
    for (size_t i = 0; i < EFFECT_PARAMETER_SIZE; i++) {
        mParams[i].setTime = 0.f;
        mParams[i].frequencyRatio = 1.f;
    }
}

PlayTimer::~PlayTimer() = default;

STDMETHODIMP_(HRESULT __stdcall) PlayTimer::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) PlayTimer::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];
    float* inBuf = static_cast<float*>(inParam.pBuffer);
    float* outBuf = static_cast<float*>(outParam.pBuffer);

    if (IsEnabled) { //有効
        if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
            outParam.BufferFlags = inParam.BufferFlags;
            return;
        }

        PlayTimerParam param = *reinterpret_cast<PlayTimerParam*>(BeginProcess());
        mCurrentTime += static_cast<unsigned>(inParam.ValidFrameCount * param.frequencyRatio);

        //波形は何もせずにそのままコピーする
        if (inBuf != outBuf) {
            memcpy(outBuf, inBuf, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }

        outParam.ValidFrameCount = inParam.ValidFrameCount;
        outParam.BufferFlags = inParam.BufferFlags;

        EndProcess();
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) PlayTimer::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(PlayTimerParam)) {
        const PlayTimerParam param = *static_cast<const PlayTimerParam*>(pParameters);
        mCurrentTime = static_cast<unsigned>(param.setTime * mInputFmt.nSamplesPerSec);
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong XAPO parameter size");
    }
}

STDMETHODIMP_(void __stdcall) PlayTimer::GetParameters(void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(float)) {
        *static_cast<float*>(pParameters) = static_cast<float>(mCurrentTime) / static_cast<float>(mInputFmt.nSamplesPerSec);
    } else {
        Debug::logWarning("Wrong XAPO parameter size");
    }
}
