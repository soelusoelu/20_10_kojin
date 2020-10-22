#include "WaveformRenderSample.h"
#include "../Sound/SoundComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform2D.h"
#include "../../Sound/Effects/SoundEffect.h"
#include "../../Sound/Effects/SoundEffectCollection.h"
#include "../../Sound/Effects/FourierTransform/DiscreteFourierTransform.h"
#include "../../Sound/Player/SoundLoop.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Sound/Volume/SoundVolume.h"
#include "../../System/Window.h"

WaveformRenderSample::WaveformRenderSample(GameObject& gameObject) :
    Component(gameObject),
    mSound(nullptr),
    mFourierID(0) {
}

WaveformRenderSample::~WaveformRenderSample() = default;

void WaveformRenderSample::start() {
    mSound = getComponent<SoundComponent>();
    mFourierID = mSound->getSoundEffect().getEffectCollection().discreteFourierTransform();
    mSound->getSoundPlayer().getLoop().loopAll();
    mSound->getSoundPlayer().playStreamingFadeIn(0.5f, 1.f);
}

void WaveformRenderSample::update() {
    mSound->getSoundEffect().getEffectParameters(mFourierID, &mFourierData, sizeof(mFourierData));
    if (mFourierData.size() == 0) {
        return;
    }

    auto pre = Vector2(0.f, Window::standardHeight() / 2.f);
    size_t loopEnd = mFourierData.size() / 2 - 1;
    auto incX = static_cast<float>(Window::standardWidth()) / static_cast<float>(loopEnd);
    for (size_t i = 0; i < loopEnd; i++) {
        //auto y = Math::clamp<unsigned>(Window::standardHeight() - fabsf(mFourierData[i].imag()) * 10.f, 0, Window::standardHeight());
        auto y = Math::clamp<unsigned>(mFourierData[i].imag() * 10.f + Window::standardHeight() / 2.f, 0, Window::standardHeight());

        auto next = Vector2(i * incX, y);
        Debug::renderLine(pre, next);
        pre = next;
    }

    auto& sv = mSound->getSoundVolume();
    auto vo = sv.getVolume();
    if (Input::keyboard().getKeyDown(KeyCode::UpArrow)) {
        sv.setVolume(vo + 0.05f);
    }
    if (Input::keyboard().getKeyDown(KeyCode::DownArrow)) {
        sv.setVolume(vo - 0.05f);
    }
}
