#include "Text.h"
#include "../../Device/DrawString.h"
#include "../../Imgui/imgui.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

Text::Text(GameObject& gameObject) :
    TextBase(gameObject),
    mText("") {
}

Text::~Text() = default;

void Text::lateUpdate() {
    TextBase::lateUpdate();

    if (!mIsActive) {
        return;
    }

    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;

    for (const auto& s : split) {
        mDrawString->drawString(s, pos, mScale, mColor, mAlpha, mPivot);
        pos.y += DrawString::HEIGHT * mScale.y;
    }
}

void Text::loadProperties(const rapidjson::Value& inObj) {
    TextBase::loadProperties(inObj);

    JsonHelper::getString(inObj, "text", &mText);
}

void Text::drawInspector() {
    TextBase::drawInspector();

    ImGui::Text("Text: %s", mText.c_str());
}

void Text::setText(const std::string & text) {
    mText = text;
}

const std::string& Text::text() const {
    return mText;
}
