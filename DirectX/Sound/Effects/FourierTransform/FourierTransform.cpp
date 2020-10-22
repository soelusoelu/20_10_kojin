#include "FourierTransform.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"
#include <vector>

FourierTransform::FourierTransform() :
    mNumberOfStage(0)
{
}

FourierTransform::~FourierTransform() = default;

void FourierTransform::initialize(unsigned size) {
    mNumberOfStage = log2(size);
    initBitRevTable(size);
}

void FourierTransform::discreteFourierTransform(std::complex<float>* out, const std::complex<float>* in, unsigned size) const {
    //本
    //for (size_t i = 0; i < size; i++) {
    //    for (size_t j = 0; j < size; j++) {
    //        auto temp = Math::TwoPI * i * j / size;
    //        float real = cosf(temp);
    //        float imag = -sinf(temp);
    //        std::complex<float> add(
    //            real * in[j].real() - imag * in[j].imag(),
    //            real * in[j].imag() + imag * in[j].real()
    //        );
    //        out[i] += add;
    //    }
    //}

    //サイト
    //for (size_t i = 0; i < size; i++) {
    //    float realSum = 0.f;
    //    float imagSum = 0.f;
    //    for (size_t j = 0; j < size; j++) {
    //        auto temp = Math::TwoPI * i * j / size;
    //        realSum += in[j].real() * cosf(temp);
    //        imagSum += in[j].real() * sinf(temp);
    //    }
    //    out[i] += std::complex<float>(realSum, imagSum);
    //}
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            out[i] += in[j].real() * exp(std::complex<float>(0.f, -Math::TwoPI * i * j / size));
        }
    }
}

void FourierTransform::fastFourierTransform(std::complex<float>* out, const std::complex<float>* in, unsigned size) const {
    //if (mNumberOfStage <= 0) {
    //    Debug::logWarning("Please initialize.");
    //    return;
    //}

    //fft
    //本
    //for (int stage = 1; stage <= mNumberOfStage; stage++) {
    //    for (int i = 0; i < pow2(stage - 1); i++) {
    //        for (int j = 0; j < pow2(mNumberOfStage - stage); j++) {
    //            int j = pow2(mNumberOfStage - stage + 1) * i + j;
    //            int m = pow2(mNumberOfStage - stage) + j;
    //            int r = pow2(stage - 1) * j;
    //            float aReal = in[j].real();
    //            float aImag = in[j].imag();
    //            float bReal = in[m].real();
    //            float bImag = in[m].imag();
    //            float cReal = cosf(Math::TwoPI * r / size);
    //            float cImag = -sinf(Math::TwoPI * r / size);

    //            out[j].real(aReal + bReal);
    //            out[j].imag(aImag + bImag);
    //            if (stage < mNumberOfStage) {
    //                out[m].real((aReal - bReal) * cReal - (aImag - bImag) * cImag);
    //                out[m].imag((aImag - bImag) * cReal + (aReal - bReal) * cImag);
    //            } else {
    //                out[m].real(aReal - bReal);
    //                out[m].imag(aImag - bImag);
    //            }
    //        }
    //    }
    //}

    //インデックスを並び替える
    //bitReverse(out, size);

    //サイト
    if (size == 1) {
        return;
    }
    std::vector<std::complex<float>> b(size / 2);
    std::vector<std::complex<float>> c(size / 2);
    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            b[i / 2] = in[i];
        }
        if (i % 2 == 1) {
            c[i / 2] = in[i];
        }
    }
    fastFourierTransform(b.data(), b.data(), b.size());
    fastFourierTransform(c.data(), c.data(), c.size());
    for (size_t i = 0; i < size; i++) {
        out[i] = b[i % (size / 2)] + c[i % (size / 2)] * exp(std::complex<float>(0.f, Math::TwoPI * i / size));
    }
}

int FourierTransform::log2(int x) const {
    int y = 0;
    while (x > 1) {
        x >>= 1;
        y++;
    }

    return y;
}

int FourierTransform::pow2(int x) const {
    int y = 0;
    if (x == 0) {
        y = 1;
    } else {
        y = 2 << (x - 1);
    }

    return y;
}

void FourierTransform::initBitRevTable(unsigned size) {
    mBitRevTable.resize(size);

    for (size_t stage = 1; stage <= mNumberOfStage; stage++) {
        for (size_t i = 0; i < pow2(stage - 1); i++) {
            mBitRevTable[pow2(stage - 1) + i] = mBitRevTable[i] + pow2(mNumberOfStage - stage);
        }
    }
}

void FourierTransform::bitReverse(std::complex<float>* fftData, unsigned size) const {
    for (size_t i = 0; i < size; i++) {
        if (mBitRevTable[i] > i) {
            float real = fftData[mBitRevTable[i]].real();
            float imag = fftData[mBitRevTable[i]].imag();
            fftData[mBitRevTable[i]].real(fftData[i].real());
            fftData[mBitRevTable[i]].imag(fftData[i].imag());
            fftData[i].real(real);
            fftData[i].imag(imag);
        }
    }
}
