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

    //ウィンドウ位置を固定
    ImGui::SetNextWindowPos(ImVec2(mInspectorPositionX, 0.f), ImGuiCond_Always);
    //ウィンドウサイズを固定
    ImGui::SetNextWindowSize(ImVec2(Window::debugWidth() - mInspectorPositionX, Window::debugHeight()), ImGuiCond_Always);
    //ウィンドウカラーを設定
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.f, 0.f, 1.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.f, 0.f, 1.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));

    ImGui::Begin("Inspector");

    drawName(*target);
    ImGui::Separator(); //区切り線
    drawTag(*target);
    ImGui::Separator(); //区切り線
    target->transform().drawInspector();
    ImGui::Separator(); //区切り線

    //全コンポーネントの情報を表示
    const auto& compList = target->componentManager().getAllComponents();
    for (const auto& comp : compList) {
        //最初はすべてのコンポーネントを開いておく
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        //コンポーネントごとに階層を作る
        if (ImGui::TreeNode(comp->getComponentName().c_str())) {
            comp->drawInspector();

            ImGui::TreePop();
        }
    }

    ImGui::End();
    ImGui::PopStyleColor(3);
}

void ImGuiInspector::drawName(const GameObject& target) const {
    const auto name = "Name: " + target.name();
    ImGui::Text(name.c_str());
}

void ImGuiInspector::drawTag(const GameObject& target) const {
    const auto tag = "Tag: " + target.tag();
    ImGui::Text(tag.c_str());
}
