#include "CharacterCost.h"
#include "../../Imgui/imgui.h"
#include "../../Math/Math.h"
#include "../../Utility/LevelLoader.h"

CharacterCost::CharacterCost(GameObject& gameObject)
    : Component(gameObject)
    , mMaxCost(0)
    , mCurrentCost(0)
{
}

CharacterCost::~CharacterCost() = default;

void CharacterCost::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "maxCost", &mMaxCost);
    mCurrentCost = mMaxCost;
}

void CharacterCost::drawInspector() {
    ImGui::SliderInt("MaxCost", &mMaxCost, 0, 100);
    ImGui::SliderInt("CurrentCost", &mCurrentCost, 0, 100);
}

void CharacterCost::useCost(int amount) {
    mCurrentCost -= amount;
    clampCost();
}

void CharacterCost::returnCost(int amount) {
    mCurrentCost += amount;
    clampCost();
}

int CharacterCost::getCost() const {
    return mCurrentCost;
}

int CharacterCost::getMaxCost() const {
    return mMaxCost;
}

void CharacterCost::clampCost() {
    mCurrentCost = Math::clamp<int>(mCurrentCost, 0, mMaxCost);
}
