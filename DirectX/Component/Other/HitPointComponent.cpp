#include "HitPointComponent.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../Device/Subject.h"
#include "../../Utility/LevelLoader.h"

HitPointComponent::HitPointComponent(GameObject& gameObject)
    : Component(gameObject)
    , mHp(0)
    , mMaxHp(0)
    , mCallbackUpdateHp(std::make_unique<Subject>())
{
}

HitPointComponent::~HitPointComponent() = default;

void HitPointComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "HP", &mHp);
    JsonHelper::getInt(inObj, "maxHP", &mMaxHp);
    if (mMaxHp < mHp) {
        mMaxHp = mHp;
    }
}

void HitPointComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setInt(alloc, inObj, "HP", mHp);
    JsonHelper::setInt(alloc, inObj, "maxHP", mMaxHp);
}

void HitPointComponent::drawInspector() {
    if (ImGuiWrapper::sliderInt("HP", mHp, 0, mMaxHp)) {
        mCallbackUpdateHp->notify();
    }
    if (ImGuiWrapper::dragInt("MaxHP", mMaxHp, 1.f, mHp)) {
        mCallbackUpdateHp->notify();
    }
}

void HitPointComponent::takeDamage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    mHp -= damage;
    if (mHp < 0) {
        mHp = 0;
    }

    mCallbackUpdateHp->notify();
}

void HitPointComponent::takeHeal(int heal) {
    if (heal < 0) {
        heal = 0;
    }
    mHp += heal;
    clampHpIfOverMax();

    mCallbackUpdateHp->notify();
}

void HitPointComponent::setHP(int hp, bool isChangeMax) {
    mHp = hp;

    //最大値も変更するか
    if (isChangeMax) {
        mMaxHp = hp;
    }

    clampHpIfOverMax();

    mCallbackUpdateHp->notify();
}

int HitPointComponent::getHP() const {
    return mHp;
}

int HitPointComponent::getMaxHP() const {
    return mMaxHp;
}

float HitPointComponent::hpRate() const {
    return static_cast<float>(mHp) / static_cast<float>(mMaxHp);
}

void HitPointComponent::callbackUpdateHP(const std::function<void()>& callback) {
    mCallbackUpdateHp->addObserver(callback);
}

void HitPointComponent::clampHpIfOverMax() {
    if (mHp > mMaxHp) {
        mHp = mMaxHp;
    }
}
