#include "TextBase.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"

TextBase::TextBase(GameObject& gameObject) :
    Component(gameObject),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mColor(ColorPalette::white),
    mAlpha(1.f),
    mPivot(Pivot::LEFT_TOP),
    mIsActive(true) {
}

TextBase::~TextBase() = default;

void TextBase::onEnable(bool value) {
    setActive(value);
}

void TextBase::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getVector3(inObj, "color", &mColor);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    std::string src;
    if (JsonHelper::getString(inObj, "pivot", &src)) {
        PivotFunc::stringToPivot(src, &mPivot);
    }
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
}

void TextBase::drawInspector() {
    ImGuiWrapper::dragVector2("Position", mPosition, 0.1f);
    ImGuiWrapper::dragVector2("Scale", mScale, 0.01f, 0.f, FLT_MAX);
    ImGuiWrapper::colorEdit3("Color", mColor);
    ImGuiWrapper::sliderFloat("Alpha", mAlpha, 0.f, 1.f);
    ImGui::Checkbox("IsActive", &mIsActive);
}

void TextBase::setPosition(const Vector2& pos) {
    mPosition = pos;
}

const Vector2& TextBase::getPosition() const {
    return mPosition;
}

void TextBase::setScale(const Vector2& scale) {
    mScale = scale;
}

const Vector2& TextBase::getScale() const {
    return mScale;
}

void TextBase::setColor(const Vector3& color) {
    mColor = color;
}

const Vector3& TextBase::getColor() const {
    return mColor;
}

void TextBase::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TextBase::getAlpha() const {
    return mAlpha;
}

void TextBase::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot TextBase::getPivot() const {
    return mPivot;
}

void TextBase::setActive(bool value) {
    mIsActive = value;
}

bool TextBase::getActive() const {
    return mIsActive;
}

void TextBase::setDrawString(DrawString* drawString) {
    mDrawString = drawString;
}
