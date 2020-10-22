#include "MeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DebugLayer/Debug.h"
#include "../../Device/AssetsManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../System/World.h"
#include "../../Utility/LevelLoader.h"

MeshComponent::MeshComponent(GameObject& gameObject) :
    Component(gameObject),
    mMesh(nullptr),
    mFileName(),
    mState(State::ACTIVE),
    mAlpha(1.f) {
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    setActive(gameObject().getActive());
}

void MeshComponent::onEnable(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    //ファイル名からメッシュを生成
    if (JsonHelper::getString(inObj, "fileName", &mFileName)) {
        mMesh = World::instance().assetsManager().createMesh(mFileName);
        addToManager();
    }

    std::string shader;
    //シェーダー名が取得できたら読み込む
    if (!JsonHelper::getString(inObj, "shaderName", &shader)) {
        //できなかったらデフォルトを使う
        shader = "Mesh.hlsl";
        //テクスチャが有るなら
        if (mMesh->getMaterial(0).texture) {
            shader = "MeshTexture.hlsl";
        }
        //ノーマルマップが有るなら
        if (mMesh->getMaterial(0).mapTexture) {
            shader = "NormalMap.hlsl";
        }
    }
    //シェーダーを生成する
    mMesh->loadShader(shader);

    //アルファ値を取得する
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Alpha", mAlpha);
}

void MeshComponent::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    TransparentConstantBuffer meshcb;
    const auto& world = transform().getWorldTransform();
    meshcb.world = world;
    meshcb.wvp = world * camera.getViewProjection();
    meshcb.lightDir = dirLight.getDirection();
    meshcb.cameraPos = camera.getPosition();
    mMesh->setShaderData(&meshcb, sizeof(meshcb), 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        MaterialConstantBuffer matcb;
        const auto& mat = mMesh->getMaterial(i);
        matcb.ambient = mat.ambient;
        //アルファ値は0のときが多いから
        float alpha = mAlpha;
        if (!Math::nearZero(mat.transparency)) {
            alpha *= mat.transparency;
        }
        matcb.diffuse = Vector4(mat.diffuse, alpha);
        matcb.specular = mat.specular;
        //データ転送
        mMesh->setShaderData(&matcb, sizeof(matcb), 1);

        //テクスチャが有るなら登録
        if (mat.texture) {
            mat.texture->setTextureInfo();
        }

        //描画
        mMesh->draw(i);
    }
}

void MeshComponent::destroy() {
    mState = State::DEAD;
}

void MeshComponent::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool MeshComponent::getActive() const {
    return mState == State::ACTIVE;
}

bool MeshComponent::isDead() const {
    return mState == State::DEAD;
}

const Vector3& MeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float MeshComponent::getRadius() const {
    return mMesh->getRadius();
}

void MeshComponent::setAlpha(float alpha) {
    mAlpha = alpha;
}

float MeshComponent::getAlpha() const {
    return mAlpha;
}

void MeshComponent::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshComponent::addToManager() {
    //マネージャーが登録されていなかったら終了する
    if (!mMeshManager) {
        Debug::logWarning("The mesh manager is not registered.");
        return;
    }

    //マネージャーに自身を登録する
    mMeshManager->add(shared_from_this());
}
