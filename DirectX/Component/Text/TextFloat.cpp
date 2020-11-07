#include "TextFloat.h"
#include "../../Device/DrawString.h"
#include "../../Imgui/imgui.h"
#include "../../Utility/LevelLoader.h"

TextFloat::TextFloat(GameObject& gameObject) :
    TextBase(gameObject),
    mNumber(0.f),
    mDecimalDigits(1) {
}

TextFloat::~TextFloat() = default;

void TextFloat::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    mDrawString->drawNumber(mNumber, mPosition, mScale, mDecimalDigits, mColor, mAlpha, mPivot);
}

void TextFloat::loadProperties(const rapidjson::Value& inObj) {
    TextBase::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "number", &mNumber);
    JsonHelper::getInt(inObj, "decimalDigits", &mDecimalDigits);
}

void TextFloat::drawInspector() {
    TextBase::drawInspector();

    ImGui::Text("Text: %f", mNumber);
    ImGui::Text("DecimalDigits: %d", mDecimalDigits);
}

void TextFloat::setNumber(float number) {
    mNumber = number;
}

float TextFloat::number() const {
    return mNumber;
}

void TextFloat::setDecimalDigits(int decimalDigits) {
    mDecimalDigits = decimalDigits;
}

int TextFloat::getDecimalDigits() const {
    return mDecimalDigits;
}
