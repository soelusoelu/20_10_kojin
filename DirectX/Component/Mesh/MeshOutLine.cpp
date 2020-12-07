#include "MeshOutLine.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DebugLayer/Debug.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Imgui/imgui.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/AssetsManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshOutLine::MeshOutLine(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mDrawer(nullptr)
    , mOutLineShader(nullptr)
    , mSkinMesh(nullptr)
    , mOutLineColor(ColorPalette::white)
    , mOutLineThickness(0.1f)
    , mIsDrawOutLine(true)
    , mIsAnimation(false)
    , mOffset(0.f)
{
}

MeshOutLine::~MeshOutLine() = default;

void MeshOutLine::start() {
    auto mesh = getComponent<MeshComponent>();

    //描画インターフェース取得
    mMesh = mesh->getMesh();
    mDrawer = mesh->getDrawer();

    //ボーンの有無でアニメーションするモデルか判断する
    mIsAnimation = (mesh->getAnimation()->getBoneCount() > 0);

    //アニメーションするかでシェーダーを決める
    auto name = (mIsAnimation) ? "SkinMeshOutLine.hlsl" : "OutLine.hlsl";
    mOutLineShader = AssetsManager::instance().createShader(name);

    if (mIsAnimation) {
        mSkinMesh = getComponent<SkinMeshComponent>();
    }

    //描画前描画の登録
    getComponent<MeshRenderer>()->setDrawBefore(this);
}

void MeshOutLine::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "outLineColor", &mOutLineColor);
    JsonHelper::getFloat(inObj, "outLineColorThickness", &mOutLineThickness);
    JsonHelper::getBool(inObj, "isDrawOutLine", &mIsDrawOutLine);
    JsonHelper::getFloat(inObj, "offset", &mOffset);
}

void MeshOutLine::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setVector3(alloc, inObj, "outLineColor", mOutLineColor);
    JsonHelper::setFloat(alloc, inObj, "outLineColorThickness", mOutLineThickness);
    JsonHelper::setBool(alloc, inObj, "isDrawOutLine", mIsDrawOutLine);
    JsonHelper::setFloat(alloc, inObj, "offset", mOffset);
}

void MeshOutLine::drawInspector() {
    ImGuiWrapper::colorEdit3("OutLineColor", mOutLineColor);
    ImGui::SliderFloat("OutLineThickness", &mOutLineThickness, 0.f, 1.f);
    ImGui::Checkbox("IsDrawOutLine", &mIsDrawOutLine);
    ImGuiWrapper::dragFloat("Offset", mOffset, 0.01f);
}

void MeshOutLine::drawBefore(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mIsDrawOutLine) {
        //裏面のみ描画したいから
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::FRONT);

        //アウトライン描画
        drawOutLine(camera, dirLight);

        //設定を戻す
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);
    }
}

void MeshOutLine::setOutLineColor(const Vector3& color) {
    mOutLineColor = color;
}

const Vector3& MeshOutLine::getOutLineColor() const {
    return mOutLineColor;
}

void MeshOutLine::setOutLineThickness(float thickness) {
    mOutLineThickness = thickness;
}

float MeshOutLine::getOutLineThickness() const {
    return mOutLineThickness;
}

void MeshOutLine::setActiveOutLine(bool value) {
    mIsDrawOutLine = value;
}

bool MeshOutLine::getActiveOutLine() const {
    return mIsDrawOutLine;
}

void MeshOutLine::drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const {
    //アウトラインシェーダーの登録
    mOutLineShader->setShaderInfo();

    //スケールを拡大したモデルをアウトラインとして描画するため
    //ワールド行列の再計算をする
    const auto& t = transform();
    auto offset = Vector3::up * mOffset;
    auto world = Matrix4::createTranslation(-t.getPivot() - offset);
    world *= Matrix4::createScale(t.getScale() + Vector3::one * mOutLineThickness);
    world *= Matrix4::createFromQuaternion(t.getRotation());
    world *= Matrix4::createTranslation(t.getPosition() + offset * t.getScale());

    //シェーダーのコンスタントバッファーに各種データを渡す
    OutLineConstantBuffer outlinecb;
    outlinecb.wvp = world * camera.getViewProjection();
    outlinecb.outlineColor = Vector4(mOutLineColor, 1.f);
    mOutLineShader->transferData(&outlinecb, sizeof(outlinecb), 0);

    //アニメーションするならボーンのデータも渡す
    if (mIsAnimation) {
        //ボーンデータを転送する
        mOutLineShader->transferData(mSkinMesh->getBoneCurrentFrameMatrix().data(), sizeof(SkinMeshConstantBuffer), 1);
    }

    //アウトラインを描画する
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        mDrawer->draw(i);
    }
}
