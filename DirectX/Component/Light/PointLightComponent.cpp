#include "PointLightComponent.h"
#include "../Camera/Camera.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Imgui/imgui.h"
#include "../../Light/LightManager.h"
#include "../../Light/PointLight.h"
#include "../../Mesh/IMeshLoader.h"
#include "../../Mesh/Material.h"
#include "../../System/Window.h"
#include "../../System/Shader/Shader.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

PointLightComponent::PointLightComponent(GameObject& gameObject) :
    Component(gameObject),
    mLightColor(ColorPalette::white),
    mInnerRadius(0.5f),
    mOuterRadius(1.f),
    mIntensity(1.f) {
}

PointLightComponent::~PointLightComponent() = default;

void PointLightComponent::start() {
    if (mLightManager) {
        mLightManager->addPointLight(shared_from_this());
    }
}

void PointLightComponent::finalize() {
    if (mLightManager) {
        mLightManager->removePointLight(shared_from_this());
    }
}

void PointLightComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "color", &mLightColor);
    JsonHelper::getFloat(inObj, "innerRadius", &mInnerRadius);
    JsonHelper::getFloat(inObj, "outerRadius", &mOuterRadius);
    JsonHelper::getFloat(inObj, "intensity", &mIntensity);
}

void PointLightComponent::drawInspector() {
    ImGuiWrapper::colorEdit3("Color", mLightColor);
    ImGui::SliderFloat("InnerRadius", &mInnerRadius, 0.01f, mOuterRadius);
    ImGui::SliderFloat("OuterRadius", &mOuterRadius, mInnerRadius, 100.f);
    ImGui::SliderFloat("Intensity", &mIntensity, 0.f, 10.f);
}

void PointLightComponent::draw(const Camera& camera, const PointLight& pointLight) const {
    //auto scale = Matrix4::createScale(transform().getScale() * mOuterRadius / pointLight.radius);
    //auto trans = Matrix4::createTranslation(transform().getPosition());
    //auto world = scale * trans;

    //auto shader = pointLight.shader;

    ////シェーダーのコンスタントバッファーに各種データを渡す
    //PointLightConstantBuffer cb;
    //cb.wvp = world * camera.getViewProjection();
    //cb.worldPos = transform().getPosition();
    //cb.cameraPos = camera.getPosition();
    //cb.windowSize = Vector2(Window::width(), Window::height());
    //cb.diffuseColor = mLightColor;
    //cb.innerRadius = mInnerRadius;
    //cb.outerRadius = mOuterRadius;
    //cb.intensity = mIntensity;

    ////シェーダーにデータ転送
    //shader->transferData(&cb, sizeof(cb));

    //auto mesh = pointLight.mesh;
    //auto mats = pointLight.materials;
    ////マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    //for (size_t i = 0; i < mats.size(); i++) {
    //    //使用されていないマテリアル対策
    //    if (mats[i]->numIndices == 0) {
    //        continue;
    //    }
    //    //インデックスバッファーをセット
    //    mesh->getVertexArray()->setIndexBuffer(i);

    //    //プリミティブをレンダリング
    //    DirectX::instance().drawIndexed(mats[i]->numIndices);
    //}
}

void PointLightComponent::setLightColor(const Vector3& color) {
    mLightColor = color;
}

void PointLightComponent::setInnerRadius(float radius) {
    mInnerRadius = radius;
}

void PointLightComponent::setOuterRadius(float radius) {
    mOuterRadius = radius;
}

void PointLightComponent::setIntensity(float value) {
    mIntensity = value;
}

void PointLightComponent::setLightManager(LightManager* manager) {
    mLightManager = manager;
}
