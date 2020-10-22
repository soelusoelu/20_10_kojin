#include "TextNumber.h"
#include "../../Device/DrawString.h"
#include "../../Utility/LevelLoader.h"

TextNumber::TextNumber(GameObject& gameObject) :
    TextBase(gameObject),
    mNumber(0) {
}

TextNumber::~TextNumber() = default;

void TextNumber::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    mDrawString->drawNumber(mNumber, mPosition, mScale, mColor, mAlpha, mPivot);
}

void TextNumber::loadProperties(const rapidjson::Value& inObj) {
    TextBase::loadProperties(inObj);

    JsonHelper::getInt(inObj, "number", &mNumber);
}

void TextNumber::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    TextBase::drawDebugInfo(inspect);

    inspect->emplace_back("Number", mNumber);
}

void TextNumber::setNumber(int number) {
    mNumber = number;
}

int TextNumber::number() const {
    return mNumber;
}
