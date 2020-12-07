#include "MeshRenderer.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "ShadowMap.h"
#include "SkinMeshComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Mesh/MeshManager.h"

MeshRenderer::MeshRenderer(GameObject& gameObject)
    : Component(gameObject)
    , mBeforeDrawer(nullptr)
    , mMeshComponent(nullptr)
    , mMeshShader(nullptr)
    , mShadowMap(nullptr)
{
}

MeshRenderer::~MeshRenderer() = default;

void MeshRenderer::start() {
    mMeshComponent = getComponent<MeshComponent>();

    mMeshShader = getComponent<MeshShader>();
    //MeshShaderがないなら追加する
    if (!mMeshShader) {
        mMeshShader = addComponent<MeshShader>("MeshShader");
    }
    //値をセットする
    mMeshShader->setInterface(mMeshComponent->getMesh(), mMeshComponent->getAnimation());

    const auto& shadow = gameObject().getGameObjectManager().find("Shadow");
    mShadowMap = shadow->componentManager().getComponent<ShadowMap>();

    //スキンメッシュコンポーネントを作れたら作る
    createSkinMeshComponent();

    //マネージャーに登録する
    addToManager();
}

void MeshRenderer::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    //描画できない状態なら終了
    if (!mMeshComponent->isDraw()) {
        return;
    }

    //描画前描画インターフェースがセットされてたら描画
    if (mBeforeDrawer) {
        mBeforeDrawer->drawBefore(camera, dirLight);
    }

    //メッシュ描画
    drawMesh(camera, dirLight);
}

void MeshRenderer::setDrawBefore(const IDrawBefore* drawer) {
    mBeforeDrawer = drawer;
}

const MeshComponent& MeshRenderer::getMeshComponent() const {
    return *mMeshComponent;
}

MeshShader& MeshRenderer::getMeshShader() const {
    return *mMeshShader;
}

void MeshRenderer::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshRenderer::drawMesh(const Camera& camera, const DirectionalLight& dirLight) const {
    //描画前準備
    mMeshShader->preSet();

    //メッシュ共通の値を設定する
    mMeshShader->setCommonValue(camera, dirLight);

    //コンスタントバッファに転送する
    mMeshShader->transferData();

    //深度テクスチャを転送する
    mShadowMap->transferShadowTexture();

    const auto loopCount = mMeshComponent->getMesh()->getMeshCount();
    for (size_t i = 0; i < loopCount; ++i) {
        //マテリアルを設定する
        mMeshShader->setDefaultMaterial(*mMeshComponent, i);

        //描画
        mMeshComponent->getDrawer()->draw(i);
    }
}

void MeshRenderer::createSkinMeshComponent() {
    const auto iMesh = mMeshComponent->getMesh();
    const auto iAnim = mMeshComponent->getAnimation();

    if (!iMesh) {
        return;
    }
    if (!iAnim) {
        return;
    }

    //ボーンがないモデルなら終了
    if (iAnim->getBoneCount() == 0) {
        return;
    }

    auto skinMesh = getComponent<SkinMeshComponent>();
    //SkinMeshComponentがアタッチされてないなら追加する
    if (!skinMesh) {
        skinMesh = addComponent<SkinMeshComponent>("SkinMeshComponent");
        skinMesh->setValue(mMeshShader, iAnim);
    }
}

void MeshRenderer::addToManager() {
    //マネージャーが登録されていなかったら終了する
    if (!mMeshManager) {
        Debug::logWarning("The mesh manager is not registered.");
        return;
    }

    //マネージャーに自身を登録する
    mMeshManager->add(shared_from_this());
}
