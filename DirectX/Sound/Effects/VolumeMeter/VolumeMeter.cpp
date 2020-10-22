#include "VolumeMeter.h"
#include "../../../DebugLayer/Debug.h"

VolumeMeter::VolumeMeter() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mPeakLevels), sizeof(float), TRUE),
    mInputFmt(),
    mOutputFmt(),
    mMaxVolume(0.f)
{
    ZeroMemory(mPeakLevels, sizeof(mPeakLevels));
}

VolumeMeter::~VolumeMeter() = default;

STDMETHODIMP_(HRESULT __stdcall) VolumeMeter::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) VolumeMeter::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];
    float* inBuf = static_cast<float*>(inParam.pBuffer);
    float* outBuf = static_cast<float*>(outParam.pBuffer);

    if (IsEnabled) { //有効
        if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
            outParam.BufferFlags = inParam.BufferFlags;
            return;
        }

        //3配列のうち有効なアドレスの取得
        float* param = reinterpret_cast<float*>(BeginProcess());

        for (size_t i = 0; i < inParam.ValidFrameCount; i++) {
            float volume = *inBuf;

            //マイナスなら符号を反転
            if (volume < 0.f) {
                volume = -volume;
            }

            //最大ボリュームを更新していたら差し替え
            if (volume > mMaxVolume) {
                mMaxVolume = volume;
            }

            //波形は何もせずにそのままコピーする
            if (outBuf != inBuf) {
                *outBuf = *inBuf;
            }
            ++inBuf;
            ++outBuf;
        }

        *param = mMaxVolume;

        outParam.ValidFrameCount = inParam.ValidFrameCount;
        outParam.BufferFlags = inParam.BufferFlags;

        EndProcess();
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) VolumeMeter::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    Debug::logWarning("This is an effect whose parameters cannot be set.");
}

STDMETHODIMP_(void __stdcall) VolumeMeter::GetParameters(void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(float)) {
        CXAPOParametersBase::GetParameters(pParameters, ParameterByteSize);
    }
}
