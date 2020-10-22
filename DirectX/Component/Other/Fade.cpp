#include "Fade.h"
#include "../ComponentManager.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Time.h"
#include "../../Utility/LevelLoader.h"

Fade::Fade(GameObject& gameObject) :
    Component(gameObject),
    mSprite(nullptr),
    mState(State::STOP),
    mFadeSpeed(1.f),
    mIsFadeOutEnded(false),
    mIsFadeInEnded(false) {
}

Fade::~Fade() = default;

void Fade::start() {
    mSprite = getComponent<SpriteComponent>();
}

void Fade::update() {
    if (mIsFadeOutEnded) {
        mIsFadeOutEnded = false;
    }
    if (mIsFadeInEnded) {
        mIsFadeInEnded = false;
    }

    if (mState == State::FADE_OUT) {
        fadeOutUpdate();
    } else if (mState == State::FADE_IN) {
        fadeInUpdate();
    } else if (mState == State::STOP) {
        //何もしない
    }
}

void Fade::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "fadeSpeed", &mFadeSpeed);
}

void Fade::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    ComponentDebug::DebugInfo info;
    info.first = "State";
    if (mState == State::FADE_OUT) {
        info.second = "OUT";
    } else if (mState == State::FADE_IN) {
        info.second = "IN";
    } else if (mState == State::STOP) {
        info.second = "STOP";
    }
    inspect->emplace_back(info);
    inspect->emplace_back("FadeSpeed", mFadeSpeed);
}

void Fade::fadeOut() {
    mSprite->setActive(true);
    mSprite->setAlpha(0.f);
    mState = State::FADE_OUT;
}

void Fade::fadeIn() {
    mSprite->setActive(true);
    mSprite->setAlpha(1.f);
    mState = State::FADE_IN;
}

void Fade::stop() {
    mState = State::STOP;
}

void Fade::reset() {
    mSprite->setActive(false);
    mSprite->setAlpha(0.f);
    mState = State::STOP;
    mIsFadeOutEnded = false;
    mIsFadeInEnded = false;
}

void Fade::setFadeSpeed(float sec) {
    mFadeSpeed = sec;
}

bool Fade::isFadeOutEnded() const {
    return mIsFadeOutEnded;
}

bool Fade::isFadeInEnded() const {
    return mIsFadeInEnded;
}

void Fade::fadeOutUpdate() {
    auto alpha = mSprite->getColor().w;
    alpha += Time::deltaTime / mFadeSpeed;
    if (alpha > 1.f) {
        alpha = 1.f;
        mIsFadeOutEnded = true;
        stop();
    }
    mSprite->setAlpha(alpha);
}

void Fade::fadeInUpdate() {
    auto alpha = mSprite->getColor().w;
    alpha -= Time::deltaTime / mFadeSpeed;
    if (alpha < 0.f) {
        alpha = 0.f;
        mIsFadeInEnded = true;
        stop();
    }
    mSprite->setAlpha(alpha);
}
