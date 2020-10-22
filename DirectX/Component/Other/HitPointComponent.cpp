#include "HitPointComponent.h"
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

void HitPointComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("HP", mHP);
    inspect->emplace_back("MaxHP", mMaxHP);
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
