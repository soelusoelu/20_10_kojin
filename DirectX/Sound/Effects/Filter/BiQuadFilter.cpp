#include "BiQuadFilter.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"

BiQuadFilter::BiQuadFilter(FilterType type) :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mParam), sizeof(FilterParam), FALSE),
    mInputFmt(),
    mOutputFmt(),
    mType(type),
    mOldIn{ 0.f, 0.f },
    mOldOut{ 0.f, 0.f },
    mA{ 0.f, 0.f, 0.f },
    mB{ 0.f, 0.f, 0.f } {
}

BiQuadFilter::~BiQuadFilter() = default;

STDMETHODIMP_(HRESULT __stdcall) BiQuadFilter::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    //出力チャンネル数を設定
    mOutCh.resize(mOutputFmt.nChannels);

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) BiQuadFilter::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) { //有効
        computeFilter(inParam, outParam);
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) BiQuadFilter::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(FilterParam)) {
        const auto& param = *static_cast<const FilterParam*>(pParameters);
        mType = param.type;
        //フィルタ係数を計算する
        computeCoefficient(param.cutoffFrequency, param.qualityFactor, mInputFmt.nSamplesPerSec);

        //基底クラスに渡す
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong size of parameter");
    }
}

void BiQuadFilter::computeFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam) {
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

    //メイン
    for (size_t i = 0; i < inParam.ValidFrameCount; i++) {
        //新しい音を計算
        float outVolume = computeVolume(*inBuf);

        //全出力チャンネルに新しい音量を設定する
        for (const auto& ch : mOutCh) {
            *ch = outVolume;
        }

        //入力バッファを音源のチャンネル数分進める
        inBuf += mInputFmt.nChannels;
        //出力バッファを次の位置まで進める
        for (auto&& ch : mOutCh) {
            ch += mOutCh.size();
        }
    }

    outParam.ValidFrameCount = inParam.ValidFrameCount;
    outParam.BufferFlags = inParam.BufferFlags;
}

float BiQuadFilter::computeVolume(float inVolume) {
    //新しい音を計算
    float outVolume = mB[0] * inVolume + mB[1] * mOldIn[0] + mB[2] * mOldIn[1] - mA[1] * mOldOut[0] - mA[2] * mOldOut[1];

    //新しい音を記憶
    mOldIn[1] = mOldIn[0];
    mOldIn[0] = inVolume;
    mOldOut[1] = mOldOut[0];
    mOldOut[0] = outVolume;

    return outVolume;
}

void BiQuadFilter::computeCoefficient(float cutoffFrequency, float qualityFactor, unsigned sampleRate) {
    float omega = 2.f * Math::PI * cutoffFrequency / sampleRate;
    if (mType == FilterType::LOW_PASS_FILTER) {
        float alpha = sinf(omega) / (2.f * qualityFactor);

        float cosOmega = cosf(omega);
        mA[0] = 1.f + alpha;
        mA[1] = -2.f * cosOmega / mA[0];
        mA[2] = (1.f - alpha) / mA[0];
        mB[0] = (1.f - cosOmega) * 0.5f / mA[0];
        mB[1] = (1.f - cosOmega) / mA[0];
        mB[2] = mB[0]; //b0と同じ
    } else if (mType == FilterType::HIGH_PASS_FILTER) {
        float alpha = sinf(omega) / (2.f * qualityFactor);

        float cosOmega = cosf(omega);
        mA[0] = 1.f + alpha;
        mA[1] = -2.f * cosOmega / mA[0];
        mA[2] = (1.f - alpha) / mA[0];
        mB[0] = (1.f + cosOmega) * 0.5f / mA[0];
        mB[1] = -(1.f + cosOmega) / mA[0];
        mB[2] = mB[0]; //b0と同じ
    } else if (mType == FilterType::BAND_PASS_FILTER) {
        float sinOmega = sinf(omega);
        float alpha = sinOmega * sinhf(logf(2.f) / 2.f * qualityFactor * omega / sinOmega);

        mA[0] = 1.f + alpha;
        mA[1] = -2.f * cosf(omega) / mA[0];
        mA[2] = (1.f - alpha) / mA[0];
        mB[0] = alpha / mA[0];
        mB[1] = 0.f;
        mB[2] = -mB[0];
    } else if (mType == FilterType::NOTCH_FILTER) {
        float sinOmega = sinf(omega);
        float alpha = sinOmega * sinhf(logf(2.f) / 2.f * qualityFactor * omega / sinOmega);

        float cosOmega = cosf(omega);
        mA[0] = 1.f + alpha;
        mA[1] = -2.f * cosOmega / mA[0];
        mA[2] = (1.f - alpha) / mA[0];
        mB[0] = 1.f / mA[0];
        mB[1] = -2.f * cosOmega / mA[0];
        mB[2] = mB[0]; //b0と同じ
    }
}
