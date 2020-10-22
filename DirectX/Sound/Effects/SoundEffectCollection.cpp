#include "SoundEffectCollection.h"
#include "Echo/Echo.h"
#include "Equalizer/Equalizer.h"
#include "Filter/SoundFilter.h"
#include "FourierTransform/DiscreteFourierTransform.h"
#include "PlayTimer/PlayTimer.h"
#include "Reverb/Reverb.h"
#include "Reverb/SimpleReverb.h"
#include "VolumeMeter/VolumeMeter.h"
#include <cassert>

SoundEffectCollection::SoundEffectCollection(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter) :
    mEffectCreater(effectCreater),
    mFilter(std::make_unique<SoundFilter>(voice, effectCreater, effectParameter)) {
}

SoundEffectCollection::~SoundEffectCollection() = default;

int SoundEffectCollection::reverb() {
    Reverb reverb;
    return mEffectCreater.createEffect(&reverb);
}

int SoundEffectCollection::simpleReverb() {
    SimpleReverb reverb;
    return mEffectCreater.createEffect(&reverb);
}

int SoundEffectCollection::echo() {
    Echo echo;
    return mEffectCreater.createEffect(&echo);
}

int SoundEffectCollection::equalizer() {
    Equalizer equalizer;
    return mEffectCreater.createEffect(&equalizer);
}

int SoundEffectCollection::volumeMeter() {
    return mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new VolumeMeter()));
}

void SoundEffectCollection::getPlayTimer() {
    auto id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new PlayTimer()));
    assert(id == PLAY_TIMER_ID);
}

int SoundEffectCollection::discreteFourierTransform() {
    return mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new DiscreteFourierTransform()));
}

SoundFilter& SoundEffectCollection::getFilter() const {
    return *mFilter;
}
