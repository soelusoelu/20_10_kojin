#include "SineWaveGenerator.h"
#include "../../Math/Math.h"

std::vector<float> SineWaveGenerator::generate(float frequency, float sampleRate, float amplitude, unsigned length) {
    std::vector<float> sine(length);
    for (size_t i = 0; i < length; i++) {
        sine[i] = amplitude * sinf(Math::TwoPI * frequency * i / sampleRate);
    }

    return sine;
}
