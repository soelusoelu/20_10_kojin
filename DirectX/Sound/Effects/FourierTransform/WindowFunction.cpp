#include "WindowFunction.h"
#include "../../../Math/Math.h"

void WindowFunction::getHanningWave(float* out, unsigned size) {
    for (size_t i = 0; i < size; i++) {
        out[i] = hanningFormula(i, size);
    }
}

std::vector<float> WindowFunction::getHanningWave(unsigned size) {
    std::vector<float> hanning(size);
    for (size_t i = 0; i < size; i++) {
        hanning[i] = hanningFormula(i, size);
    }

    return hanning;
}

void WindowFunction::hanning(float* data, unsigned size) {
    for (size_t i = 0; i < size; i++) {
        data[i] *= hanningFormula(i, size);
    }
}

void WindowFunction::hanning(std::complex<float>* out, const float* in, unsigned size) {
    for (size_t i = 0; i < size; i++) {
        out[i].real(in[i] * hanningFormula(i, size));
    }
}

float WindowFunction::hanningFormula(unsigned n, unsigned size) {
    if (size % 2 == 0) {
        return 0.5f - 0.5f * cosf(Math::TwoPI * n / size);
    } else {
        return 0.5f - 0.5f * cosf(Math::TwoPI * (n + 0.5f) / size);
    }
}
