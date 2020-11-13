#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <any>
#include <memory>
#include <string>

class Component;
class GameObject;
class Transform3D;

//ImGuiを使用したインスペクター
class ImGuiInspector {
public:
    ImGuiInspector();
    ~ImGuiInspector();
    void loadProperties(const rapidjson::Value& inObj);
    void setTarget(const std::shared_ptr<GameObject>& target);
    void drawInspect() const;

private:
    ImGuiInspector(const ImGuiInspector&) = delete;
    ImGuiInspector& operator=(const ImGuiInspector&) = delete;

    void drawName(const GameObject& target) const;
    void drawTag(const GameObject& target) const;

private:
    std::weak_ptr<GameObject> mTarget;
    float mInspectorPositionX;
};
