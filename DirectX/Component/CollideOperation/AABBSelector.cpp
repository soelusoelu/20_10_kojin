#include "AABBSelector.h"
#include "AABBMouseScaler.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

AABBSelector::AABBSelector(GameObject& gameObject)
    : Component(gameObject)
    , mAABBMouseScaler(nullptr)
    , mCanAccess(false)
{
}

AABBSelector::~AABBSelector() = default;

void AABBSelector::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();

    mAABBMouseScaler = getComponent<AABBMouseScaler>();
}

void AABBSelector::update() {
    //マウスの左ボタンが押した瞬間じゃなければ終了
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    //編集中でないならコライダーを選択する
    if (!mAABBMouseScaler->editing()) {
        mCanAccess = !selectAABB();
    }
}

void AABBSelector::setAABBsFromMesh(const MeshComponent& mesh) {
    mColliders = mesh.getComponents<AABBCollider>();
    mAABBMouseScaler->setAABB(nullptr);

    //AABBがなければ警告
    if (mColliders.empty()) {
        Debug::logError("colliders empty.");
    }
}

bool AABBSelector::accessable() const {
    if (mAABBMouseScaler->editing()) {
        return false;
    }

    return mCanAccess;
}

bool AABBSelector::selectAABB() {
    //カメラからマウスの位置へ向かうレイを取得
    const auto& rayCameraToMousePos = mCamera->screenToRay(Input::mouse().getMousePosition());

    //すべてのコライダーとレイの衝突判定
    for (size_t i = 0; i < mColliders.size(); ++i) {
        if (Intersect::intersectRayAABB(rayCameraToMousePos, mColliders[i]->getAABB())) {
            mAABBMouseScaler->setAABB(mColliders[i]);
            return true;
        }
    }

    //どのコライダーとも衝突しなかった
    return false;
}
