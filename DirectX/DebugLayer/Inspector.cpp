#include "Inspector.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../Device/DrawString.h"
#include "../GameObject/GameObject.h"
#include "../System/Window.h"
#include "../Transform/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"
#include <list>

Inspector::Inspector(DrawString* drawString) :
    mDrawString(drawString),
    mInspectorPositionX(0.f),
    mTagScale(Vector2::one),
    mElementScale(Vector2::one),
    mTransformPosition(Vector2::zero),
    mComponentPosition(Vector2::zero),
    mElementPositionX(0.f),
    mValuePositionX(0.f),
    mOffsetCharCountX(0),
    mOffsetX(0.f),
    mCharWidth(0.f),
    mCharHeight(0.f),
    mMaxElementCharCount(0) {
}

Inspector::~Inspector() = default;

void Inspector::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["inspector"];
    if (obj.IsObject()) {
        JsonHelper::getFloat(obj, "inspectorPositionX", &mInspectorPositionX);
        JsonHelper::getVector2(obj, "tagScale", &mTagScale);
        JsonHelper::getVector2(obj, "elementScale", &mElementScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
    }
}

void Inspector::initialize() {
    //1文字のサイズを基準に描画位置を決める
    mCharWidth = DrawString::WIDTH * mElementScale.x;
    mCharHeight = DrawString::HEIGHT * mElementScale.y;

    mOffsetX = mCharWidth * mOffsetCharCountX;
    mTransformPosition = Vector2(mInspectorPositionX + mOffsetX, DrawString::HEIGHT * mTagScale.y + mCharHeight);
    mComponentPosition = mTransformPosition;
    mComponentPosition.y += mCharHeight * 5;
    mElementPositionX = mTransformPosition.x + mCharWidth * 2;
    mValuePositionX = mTransformPosition.x + mCharWidth * 14;
    mMaxElementCharCount = (mValuePositionX - mElementPositionX) / mCharWidth - 1;
}

void Inspector::setTarget(const GameObjectPtr& target) {
    mTarget = target;
}

void Inspector::drawInspect() const {
    auto actor = mTarget.lock();
    if (!actor) {
        return;
    }

    drawTag(*actor);
    drawTransform(actor->transform());

    const auto& compList = actor->componentManager().getAllComponents();
    //アクターがコンポーネントを所持していなければ終了
    if (compList.empty()) {
        return;
    }

    auto drawPos = mComponentPosition;
    //全コンポーネントの情報を表示
    for (const auto& comp : compList) {
        drawComponent(*comp, &drawPos);
        drawPos.x = mComponentPosition.x;
        drawPos.y += mCharHeight * 2;
    }
}

void Inspector::drawTag(const GameObject& target) const {
    auto tag = target.tag();
    auto pos = Vector2(mInspectorPositionX + (Window::debugWidth() - mInspectorPositionX) / 2.f, 0.f);
    pos.x -= DrawString::WIDTH * mTagScale.x * tag.length() / 2.f;
    mDrawString->drawString(tag, pos, mTagScale);
}

void Inspector::drawTransform(const Transform3D& target) const {
    auto pos = mTransformPosition;
    mDrawString->drawString("Transform", pos, mElementScale);
    pos.x = mElementPositionX;
    pos.y += mCharHeight;
    //Transformの各要素を描画
    drawPosition(target, pos);
    pos.y += mCharHeight;
    drawRotation(target, pos);
    pos.y += mCharHeight;
    drawScale(target, pos);
}

void Inspector::drawPosition(const Transform3D& target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Position", pos, mElementScale);
    pos.x = mValuePositionX;
    auto tPos = target.getPosition();
    mDrawString->drawString(vector3ToString(tPos), pos, mElementScale);
}

void Inspector::drawRotation(const Transform3D& target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Rotation", pos, mElementScale);
    pos.x = mValuePositionX;
    auto rot = target.getRotation().euler();
    mDrawString->drawString(vector3ToString(rot), pos, mElementScale);
}

void Inspector::drawScale(const Transform3D& target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Scale", pos, mElementScale);
    pos.x = mValuePositionX;
    auto scale = target.getScale();
    mDrawString->drawString(vector3ToString(scale), pos, mElementScale);
}

void Inspector::drawComponent(const Component& component, Vector2 * position) const {
    auto pos = *position;
    mDrawString->drawString(component.getComponentName(), pos, mElementScale);

    //コンポーネントのデバッグ情報を取得
    ComponentDebug::DebugInfoList debugInfo;
    component.drawDebugInfo(&debugInfo);

    //すべてのデバッグ情報を描画
    for (const auto& info : debugInfo) {
        pos.x = mElementPositionX;
        auto ele = info.first;
        if (ele.length() > mMaxElementCharCount) {
            ele = ele.substr(0, mMaxElementCharCount);
        }
        pos.y += mCharHeight;
        mDrawString->drawString(ele, pos, mElementScale);
        pos.x = mValuePositionX;
        mDrawString->drawString(anyToString(info.second), pos, mElementScale);
    }

    *position = pos;
}

std::string Inspector::anyToString(const std::any& src) const {
    if (!src.has_value()) {
        return "";
    }

    const auto& type = src.type();
    std::string str;
    if (type == typeid(int)) {
        auto value = std::any_cast<int>(src);
        str = StringUtil::intToString(value);
    } else if (type == typeid(float)) {
        auto value = std::any_cast<float>(src);
        str = StringUtil::floatToString(value);
    } else if (type == typeid(bool)) {
        auto value = std::any_cast<bool>(src);
        str = StringUtil::boolToString(value);
    } else if (type == typeid(const char*)) {
        str = std::any_cast<const char*>(src);
    } else if (type == typeid(std::string)) {
        str = std::any_cast<std::string>(src);
    } else if (type == typeid(Vector2)) {
        auto value = std::any_cast<Vector2>(src);
        str = vector2ToString(value);
    } else if (type == typeid(Vector3)) {
        auto value = std::any_cast<Vector3>(src);
        str = vector3ToString(value);
    } else if (type == typeid(Vector4)) {
        auto value = std::any_cast<Vector4>(src);
        str = vector4ToString(value);
    } else if (type == typeid(Quaternion)) {
        auto value = std::any_cast<Quaternion>(src);
        str = quaternionToString(value);
    } else {
        Debug::logWarning(src.type().name());
    }

    return str;
}

std::string Inspector::vector2ToString(const Vector2& vec) const {
    auto x = "X " + minusPosition(vec.x);
    x.resize(10);
    auto y = "Y " + minusPosition(vec.y);
    return x + y;
}

std::string Inspector::vector3ToString(const Vector3& vec) const {
    auto x = "X " + minusPosition(vec.x);
    x.resize(10);
    auto y = "Y " + minusPosition(vec.y);
    y.resize(10);
    auto z = "Z " + minusPosition(vec.z);
    return x + y + z;
}

std::string Inspector::vector4ToString(const Vector4& vec) const {
    auto x = "X " + minusPosition(vec.x);
    x.resize(10);
    auto y = "Y " + minusPosition(vec.y);
    y.resize(10);
    auto z = "Z " + minusPosition(vec.z);
    z.resize(10);
    auto w = "W " + minusPosition(vec.w);
    return x + y + z + w;
}

std::string Inspector::quaternionToString(const Quaternion& quaternion) const {
    auto x = "X " + minusPosition(quaternion.x);
    x.resize(10);
    auto y = "Y " + minusPosition(quaternion.y);
    y.resize(10);
    auto z = "Z " + minusPosition(quaternion.z);
    z.resize(10);
    auto w = "W " + minusPosition(quaternion.w);
    return x + y + z + w;
}

std::string Inspector::minusPosition(float value) const {
    auto s = StringUtil::floatToString(value);
    return (value < 0.f) ? s : " " + s;
}
