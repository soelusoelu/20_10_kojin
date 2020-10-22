#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <any>
#include <memory>
#include <string>

class Component;
class DrawString;
class GameObject;
class Transform3D;

class Inspector {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    Inspector(DrawString* drawString);
    ~Inspector();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void setTarget(const GameObjectPtr& target);
    void drawInspect() const;

private:
    void drawTag(const GameObject& target) const;
    void drawTransform(const Transform3D& target) const;
    void drawPosition(const Transform3D& target, const Vector2& position) const;
    void drawRotation(const Transform3D& target, const Vector2& position) const;
    void drawScale(const Transform3D& target, const Vector2& position) const;
    void drawComponent(const Component& component, Vector2* position) const;

    std::string anyToString(const std::any& src) const;
    std::string vector2ToString(const Vector2& vec) const;
    std::string vector3ToString(const Vector3& vec) const;
    std::string vector4ToString(const Vector4& vec) const;
    std::string quaternionToString(const Quaternion& quaternion) const;
    std::string minusPosition(float value) const;

private:
    DrawString* mDrawString;
    std::weak_ptr<GameObject> mTarget;
    float mInspectorPositionX;
    Vector2 mTagScale;
    Vector2 mElementScale;
    Vector2 mTransformPosition;
    Vector2 mComponentPosition;
    //変数名のX軸の位置
    float mElementPositionX;
    //値のX軸の位置
    float mValuePositionX;
    //表示位置を右にずらす文字数
    int mOffsetCharCountX;
    //表示位置を右にずらす分量
    float mOffsetX;
    //1文字の大きさ
    float mCharWidth;
    float mCharHeight;
    //変数名の表示限界文字数
    int mMaxElementCharCount;
};
