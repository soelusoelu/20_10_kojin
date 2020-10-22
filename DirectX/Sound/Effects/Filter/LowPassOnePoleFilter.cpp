#include "LowPassOnePoleFilter.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"

MyFilter::LowPassOnePoleFilter::LowPassOnePoleFilter() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mFrequency), sizeof(float), FALSE),
    mInputFmt(),
    mOutputFmt(),
    mFrequency{ 1.f, 1.f, 1.f },
    mA1(0.f),
    mB0(0.f),
    mLastVolume(0.f) {
}

MyFilter::LowPassOnePoleFilter::~LowPassOnePoleFilter() = default;

STDMETHODIMP_(HRESULT __stdcall) MyFilter::LowPassOnePoleFilter::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    //出力チャンネル数を設定
    mOutCh.resize(mOutputFmt.nChannels);

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) MyFilter::LowPassOnePoleFilter::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) { //有効
        lowPassOnePoleFilter(inParam, outParam);
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) MyFilter::LowPassOnePoleFilter::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(float)) {
        const auto freq = *static_cast<const float*>(pParameters);
        //ローパス係数の計算
        mA1 = expf(-Math::PI * freq);
        mB0 = 1.f - mA1;

        //基底クラスに渡す
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong size of parameter");
    }
}

void MyFilter::LowPassOnePoleFilter::lowPassOnePoleFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam) {
    //サイレントなら計算はしない
    if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
        outParam.BufferFlags = inParam.BufferFlags;
        return;
    }

    //バッファの取得
    float* inBuf = static_cast<float*>(inParam.pBuffer);
    float* outBuf = static_cast<float*>(outParam.pBuffer);
    //チャンネル配列を更新
    for (size_t i = 0; i < mOutCh.size(); i++) {
        mOutCh[i] = outBuf + i;
    }
    //3配列のうち有効な値を取得
    float frequency = *reinterpret_cast<float*>(BeginProcess());

    float volume = mLastVolume;
    //サンプル数分まわす
    for (size_t i = 0; i < inParam.ValidFrameCount; i++) {
        //新しい音量を決定
        volume = volume * mA1 + *inBuf * mB0;

        //全出力チャンネルに新しい音量を設定する
        for (const auto& ch : mOutCh) {
            *ch = volume;
        }

        //入力バッファを音源のチャンネル数分進める
        inBuf += mInputFmt.nChannels;
        //出力バッファを次の位置まで進める
        for (auto&& ch : mOutCh) {
            ch += mOutCh.size();
        }
    }

    //最後の音量を保存
    mLastVolume = volume;
    outParam.ValidFrameCount = inParam.ValidFrameCount;
    outParam.BufferFlags = inParam.BufferFlags;

    EndProcess();
}
