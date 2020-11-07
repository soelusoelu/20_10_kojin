#include "ImGuiInspector.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../GameObject/GameObject.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../System/Window.h"
#include "../Transform/Transform3D.h"
#include "../Utility/LevelLoader.h"

ImGuiInspector::ImGuiInspector()
    : mInspectorPositionX(0.f)
{
}

ImGuiInspector::~ImGuiInspector() = default;

void ImGuiInspector::loadProperties(const rapidjson::Value& inObj) {
    const auto& obj = inObj["inspector"];
    if (obj.IsObject()) {
        JsonHelper::getFloat(obj, "inspectorPositionX", &mInspectorPositionX);
    }
}

void ImGuiInspector::setTarget(const std::shared_ptr<GameObject>& target) {
    mTarget = target;
}

void ImGuiInspector::drawInspect() const {
    const auto& target = mTarget.lock();
    if (!target) {
        return;
    }

    //ウィンドウ位置を設定
    ImGui::SetWindowPos(ImVec2(mInspectorPositionX, 0.f), ImGuiCond_Once);
    //ウィンドウサイズを設定
    ImGui::SetWindowSize(ImVec2(Window::debugWidth() - mInspectorPositionX, Window::debugHeight()), ImGuiCond_Once);

    ImGui::Begin("Inspector");

    drawName(*target);
    ImGui::Separator(); //区切り線
    drawTag(*target);
    ImGui::Separator(); //区切り線
    drawTransform(target->transform());
    ImGui::Separator(); //区切り線

    const auto& compList = target->componentManager().getAllComponents();
    //ゲームオブジェクトがコンポーネントを所持していなければ終了
    if (compList.empty()) {
        return;
    }

    //全コンポーネントの情報を表示
    for (const auto& comp : compList) {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        //コンポーネントごとに階層を作る
        if (ImGui::TreeNode(comp->getComponentName().c_str())) {
            drawComponent(*comp);

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void ImGuiInspector::drawName(const GameObject& target) const {
    const auto name = "Name: " + target.name();
    ImGui::Text(name.c_str());
}

void ImGuiInspector::drawTag(const GameObject& target) const {
    const auto tag = "Tag: " + target.tag();
    ImGui::Text(tag.c_str());
}

void ImGuiInspector::drawTransform(const Transform3D& target) const {
    ImGui::Text("Transform");

    //Transformの各要素を描画
    drawPosition(target);
    drawRotation(target);
    drawScale(target);
}

void ImGuiInspector::drawPosition(const Transform3D& target) const {
    const auto& pos = target.getPosition();
    ImGui::Text("    Position: X %.4f   Y %.4f   Z %.4f", pos.x, pos.y, pos.z);
}

void ImGuiInspector::drawRotation(const Transform3D& target) const {
    const auto& euler = target.getRotation().euler();
    ImGui::Text("    Rotation: X %.4f   Y %.4f   Z %.4f", euler.x, euler.y, euler.z);
}

void ImGuiInspector::drawScale(const Transform3D& target) const {
    const auto& scale = target.getScale();
    ImGui::Text("    Scale:    X %.4f   Y %.4f   Z %.4f", scale.x, scale.y, scale.z);
}

void ImGuiInspector::drawComponent(Component& component) const {
    //コンポーネントのインスペクター情報を描画
    component.drawInspector();
}
