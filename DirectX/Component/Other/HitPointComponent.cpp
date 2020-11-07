#include "HitPointComponent.h"
#include "../../Imgui/imgui.h"
#include "../../Utility/LevelLoader.h"

HitPointComponent::HitPointComponent(GameObject& gameObject) :
    Component(gameObject),
    mHP(0),
    mMaxHP(0) {
}

HitPointComponent::~HitPointComponent() = default;

void HitPointComponent::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getInt(inObj, "HP", &mHP);
    JsonHelper::getInt(inObj, "maxHP", &mMaxHP);
}

void HitPointComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setInt(alloc, inObj, "HP", mHP);
    JsonHelper::setInt(alloc, inObj, "maxHP", mMaxHP);
}

void HitPointComponent::drawInspector() {
    ImGui::SliderInt("HP", &mHP, 0, mMaxHP);
    ImGui::SliderInt("MaxHP", &mMaxHP, 0, INT_MAX);
}

void HitPointComponent::takeDamage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    mHP -= damage;
    if (mHP < 0) {
        mHP = 0;
    }
}

void HitPointComponent::takeHeal(int heal) {
    if (heal < 0) {
        heal = 0;
    }
    mHP += heal;
    clampHpIfOverMax();
}

void HitPointComponent::setHP(int hp, bool isChangeMax) {
    mHP = hp;
    if (isChangeMax) {
        mMaxHP = hp;
    } else {
        clampHpIfOverMax();
    }
}

int HitPointComponent::hp() const {
    return mHP;
}

float HitPointComponent::hpRate() const {
    return static_cast<float>(mHP) / static_cast<float>(mMaxHP);
}

void HitPointComponent::clampHpIfOverMax() {
    if (mHP > mMaxHP) {
        mHP = mMaxHP;
    }
}
