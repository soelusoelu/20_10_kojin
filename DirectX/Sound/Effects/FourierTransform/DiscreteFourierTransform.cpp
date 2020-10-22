#include "DiscreteFourierTransform.h"
#include "FourierTransform.h"
#include "WindowFunction.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"
#include <algorithm>

DiscreteFourierTransform::DiscreteFourierTransform() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mParam), sizeof(ComplexArray), TRUE),
    mFourier(std::make_unique<FourierTransform>()),
    mInputFmt(),
    mOutputFmt() {
}

DiscreteFourierTransform::~DiscreteFourierTransform() = default;

STDMETHODIMP_(HRESULT __stdcall) DiscreteFourierTransform::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    mFourier->initialize(N);
    mComp.resize(N);
    mOutComp.resize(N);

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) DiscreteFourierTransform::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) {
        //高速フーリエ変換
        discreteFourierTransform(inParam, outParam);
    }

    //波形はそのまま
    if (inParam.pBuffer != outParam.pBuffer) {
        memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
    }
}

STDMETHODIMP_(void __stdcall) DiscreteFourierTransform::GetParameters(void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(ComplexArray)) {
        auto& param = *static_cast<ComplexArray*>(pParameters);
        if (param.size() != N) {
            param.resize(N);
        }
        //フーリエ変換した波形を全コピーする
        std::copy(mOutComp.begin(), mOutComp.end(), param.begin());

        //本来なら基底クラスのGetParametersを呼ぶが、今回はポインタ(vector)を扱っている関係上ハックしてる
    } else {
        Debug::logWarning("Wrong XAPO parameter size");
    }
}

void DiscreteFourierTransform::discreteFourierTransform(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam) {
    //サイレントなら計算はしない
    if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
        outParam.BufferFlags = inParam.BufferFlags;
        return;
    }

    //バッファの取得
    float* inBuf = static_cast<float*>(inParam.pBuffer);

    //波形に窓関数を掛ける
    WindowFunction::hanning(mComp.data(), inBuf, N);

    //高速フーリエ変換
    mFourier->fastFourierTransform(mOutComp.data(), mComp.data(), N);
}
