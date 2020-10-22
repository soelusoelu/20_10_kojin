#include "SoundFilter.h"
#include "BiQuadFilter.h"
#include "HighPassOnePoleFilter.h"
#include "LowPassOnePoleFilter.h"
#include "../../Voice/VoiceDetails.h"
#include "../../../Math/Math.h"

SoundFilter::SoundFilter(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter) :
    mEffectCreater(effectCreater),
    mEffectParameter(effectParameter),
    mVoice(voice),
    mFilterID(-1) {
}

SoundFilter::~SoundFilter() = default;

void SoundFilter::lowPassFilter(float cutoffFrequency, float qualityFactor) {
    updateFilter(FilterType::LOW_PASS_FILTER, cutoffFrequency, qualityFactor);
}

void SoundFilter::lowPassFilterFromRadianFrequency(float radianFrequency, float qualityFactor) {
    float f = radianFrequencyToCutoffFrequency(radianFrequency);
    updateFilter(FilterType::LOW_PASS_FILTER, f, qualityFactor);
}

void SoundFilter::lowPassOnePoleFilter(float frequency) {
    createOnePoleFilter(FilterType::LOW_PASS_ONE_POLE_FILTER, frequency);
}

void SoundFilter::highPassFilter(float cutoffFrequency, float qualityFactor) {
    updateFilter(FilterType::HIGH_PASS_FILTER, cutoffFrequency, qualityFactor);
}

void SoundFilter::highPassOnePoleFilter(float frequency) {
    createOnePoleFilter(FilterType::HIGH_PASS_ONE_POLE_FILTER, frequency);
}

void SoundFilter::bandPassFilter(float cutoffFrequency, float qualityFactor) {
    updateFilter(FilterType::BAND_PASS_FILTER, cutoffFrequency, qualityFactor);
}

void SoundFilter::notchFilter(float cutoffFrequency, float qualityFactor) {
    updateFilter(FilterType::NOTCH_FILTER, cutoffFrequency, qualityFactor);
}

void SoundFilter::updateFilter(FilterType type, float cutoffFrequency, float qualityFactor) {
    //フィルター未作成なら作成
    if (mFilterID == -1) {
        createFilter(type);
    }

    setFilterParameters(type, cutoffFrequency, qualityFactor);
}

void SoundFilter::createFilter(FilterType type) {
    mFilterID = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new BiQuadFilter(type)));
}

void SoundFilter::createOnePoleFilter(FilterType type, float cutoffFrequency) {
    if (type == FilterType::LOW_PASS_ONE_POLE_FILTER) {
        mFilterID = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::LowPassOnePoleFilter()));
    } else if (type == FilterType::HIGH_PASS_ONE_POLE_FILTER) {
        mFilterID = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::HighPassOnePoleFilter()));
    }
    float f = Math::clamp<float>(cutoffFrequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(mFilterID, &f, sizeof(float));
}

void SoundFilter::setFilterParameters(FilterType type, float cutoffFrequency, float qualityFactor) {
    float f = clampCutoffFrequency(cutoffFrequency);
    float q = clampQ(qualityFactor);
    FilterParam param = { type, f, q };
    mEffectParameter.setEffectParameters(mFilterID, &param, sizeof(param));
}

float SoundFilter::clampCutoffFrequency(float cutoffFrequency) const {
    const unsigned s = mVoice.getVoiceDetails().sampleRate;
    //カットオフ周波数はサンプリング周波数の半分まで
    return Math::clamp<float>(cutoffFrequency, 0.f, s / 2.f);
}

float SoundFilter::clampQ(float qualityFactor) const {
    if (qualityFactor <= 0.f) { //0ちょうどでもだめ
        qualityFactor = 0.001f;
    }
    return qualityFactor;
}

float SoundFilter::cutoffFrequencyToRadianFrequency(float cutoffFrequency) const {
    return cutoffFrequency / mVoice.getVoiceDetails().sampleRate * 2.f;
}

float SoundFilter::radianFrequencyToCutoffFrequency(float radianFrequency) const {
    return radianFrequency * mVoice.getVoiceDetails().sampleRate / 2.f;
}
