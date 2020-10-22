#include "FixedDebugInformation.h"
#include "../Device/DrawString.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FixedDebugInformation::FixedDebugInformation(DrawString* drawString) :
    mDrawString(drawString),
    mScale(Vector2::one),
    mFPSPos(Vector2::zero),
    mFPS(0.f) {
}

FixedDebugInformation::~FixedDebugInformation() = default;

void FixedDebugInformation::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["fixedDebugInfo"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
    }
}

void FixedDebugInformation::initialize() {
    mFPSPos = Vector2(Window::width() / 2.f, Window::height());
}

void FixedDebugInformation::draw() const {
    auto drawPos = mFPSPos;
    std::string str = "fps";
    mDrawString->drawString(str, drawPos, mScale);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * mScale.x;
    mDrawString->drawString(StringUtil::floatToString(mFPS, 2), drawPos, mScale);
}

void FixedDebugInformation::drawFPS(float fps) {
    mFPS = fps;
}
